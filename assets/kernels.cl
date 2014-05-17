#define BLK_SIZE    16
//prefix D for decoding
#define DSHRD_LEN   (BLK_SIZE/2)
#define DSHRD_SIZE  (2*DSHRD_LEN*DSHRD_LEN)

uchar4 convertYVUtoRGBA(int y, int u, int v)
{
    uchar4 ret;
    y-=16;
    u-=128;
    v-=128;
    int b = y + (int)(1.772f*u);
    int g = y - (int)(0.344f*u + 0.714f*v);
    int r = y + (int)(1.402f*v);
    ret.x = r>255? 255 : r<0 ? 0 : r;
    ret.y = g>255? 255 : g<0 ? 0 : g;
    ret.z = b>255? 255 : b<0 ? 0 : b;
    ret.w = 255;
    return ret;
}

__kernel void nv21torgba( __global uchar4* out,
                          __global uchar*  in,
                          int    im_width,
                          int    im_height)
{
    __local uchar uvShrd[DSHRD_SIZE];
    int gx	= get_global_id(0);
    int gy	= get_global_id(1);
    int lx  = get_local_id(0);
    int ly  = get_local_id(1);
    int off = im_width*im_height;
    // every thread whose
    // both x,y indices are divisible
    // by 2 move the u,v corresponding
    // to the 2x2 block into shared mem
    int inIdx= gy*im_width+gx;
    int uvIdx= off + (gy/2)*im_width + (gx & ~1);
    int shlx = lx/2;
    int shly = ly/2;
    int shIdx= 2*(shlx+shly*DSHRD_LEN);
    if( gx%2==0 && gy%2==0 ) {
        uvShrd[shIdx+0] = in[uvIdx+0];
        uvShrd[shIdx+1] = in[uvIdx+1];
    }
    // do some work while others copy
    // uv to shared memory
    int y   = (0xFF & ((int)in[inIdx]));
    if( y < 16 ) y=16;
    barrier(CLK_LOCAL_MEM_FENCE);
    // return invalid threads
    if( gx >= im_width || gy >= im_height )
        return;
    // convert color space
    int v   = (0xFF & ((int)uvShrd[shIdx+0]));
    int u   = (0xFF & ((int)uvShrd[shIdx+1]));
    // write output to image
    out[inIdx]  = convertYVUtoRGBA(y,u,v);
}

//prefix L for laplacian
#define LHALO       1
#define LPADDING    (2*LHALO)
#define LSBLK_LEN   (BLK_SIZE+LPADDING)
#define LSBLK_SIZE  (LSBLK_LEN*LSBLK_LEN)

#define globIdx(x,y) ((y)*dim0     + (x))
#define shrdIdx(x,y) ((y)*LSBLK_LEN+ (x))

#define LOAD_TO_LOCALMEM(_gx,_gy,_lx,_ly) \
    int gx_  = clamp((_gx),0,(int)dim0-1);\
    int gy_  = clamp((_gy),0,(int)dim1-1);\
    localMem[ shrdIdx(_lx,_ly) ] = in[ globIdx(gx_,gy_) ];

__kernel void laplacian(__global uchar4* out,
                        __global uchar4*  in,
                        int    dim0,
                        int    dim1)
{
    // 0 as fast moving dimension
    // 1 as slow moving dimension
    __local uchar4 localMem[LSBLK_SIZE];
    // global indices
    int gx	= get_global_id(0);
    int gy	= get_global_id(1);
    // local indices
    int lx		= get_local_id(0);
    int ly		= get_local_id(1);
    // offset values for pulling image to local memory
    int lx2		= lx + BLK_SIZE;
    int ly2		= ly + BLK_SIZE;
    int gx2		= gx + BLK_SIZE;
    int gy2		= gy + BLK_SIZE;
    int i		= lx + LHALO;
    int j		= ly + LHALO;
    // pull image to local memory
    LOAD_TO_LOCALMEM(gx-LHALO,gy-LHALO,lx,ly);
    if (lx < LPADDING) { LOAD_TO_LOCALMEM(gx2-LHALO,gy-LHALO,lx2,ly); }
    if (ly < LPADDING) { LOAD_TO_LOCALMEM(gx-LHALO,gy2-LHALO,lx,ly2); }
    if (lx < LPADDING && ly < LPADDING) {
        LOAD_TO_LOCALMEM(gx2-LHALO,gy2-LHALO,lx2,ly2);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    float4 C   = convert_float4( localMem[ shrdIdx(i  ,j  ) ] );
    float4 N   = convert_float4( localMem[ shrdIdx(i  ,j-1) ] );
    float4 NW  = convert_float4( localMem[ shrdIdx(i-1,j-1) ] );
    float4 W   = convert_float4( localMem[ shrdIdx(i-1,j  ) ] );
    float4 SW  = convert_float4( localMem[ shrdIdx(i-1,j+1) ] );
    float4 S   = convert_float4( localMem[ shrdIdx(i  ,j+1) ] );
    float4 SE  = convert_float4( localMem[ shrdIdx(i+1,j+1) ] );
    float4 E   = convert_float4( localMem[ shrdIdx(i+1,j  ) ] );
    float4 NE  = convert_float4( localMem[ shrdIdx(i+1,j-1) ] );

    float4 acc = (float4)0.0f;
    acc = 8*C-N-NW-W-SW-S-SE-E-NE;
    acc.w = 255.0f;

    if( gx < dim0 && gy < dim1 ) {
        out[gy*dim0+gx] = convert_uchar4(acc);
    }
}

