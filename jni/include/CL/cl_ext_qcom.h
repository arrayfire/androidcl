/* Copyright (c) 2009-2012 QUALCOMM Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
 */

#ifndef __OPENCL_CL_EXT_QCOM_H
#define __OPENCL_CL_EXT_QCOM_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************************
* cl_perf_monitor_qcom extension *                                                  
**********************************/
typedef struct _cl_perf_monitor_qcom *             cl_perf_monitor_qcom;
typedef struct _cl_perf_monitor_group_id_qcom *    cl_perf_monitor_group_id_qcom;
typedef struct _cl_perf_monitor_counter_id_qcom *  cl_perf_monitor_counter_id_qcom;

typedef cl_uint                                    cl_perf_monitor_group_info_qcom;
typedef cl_uint                                    cl_perf_monitor_counter_info_qcom;
typedef cl_uint                                    cl_perf_monitor_info_qcom;
typedef cl_uint                                    cl_perf_monitor_counter_type;
            

extern CL_API_ENTRY cl_int CL_API_CALL
clGetPerfMonitorGroupInfoQCOM(cl_device_id                        /* device */,
                              cl_perf_monitor_group_id_qcom       /* group */,
                              cl_perf_monitor_group_info_qcom     /* param_name */,
                              size_t                              /* param_value_size */,
                              void *                              /* param_value */,
                              size_t *                            /* param_value_size_ret */);

extern CL_API_ENTRY cl_int CL_API_CALL
clGetPerfMonitorCounterInfoQCOM(cl_device_id                       /* device */,
                                cl_perf_monitor_counter_id_qcom    /* counter*/,
                                cl_perf_monitor_counter_info_qcom  /* param_name */,
                                size_t                             /* param_value_size */,
                                void *                             /* param_value */,
                                size_t *                           /* param_value_size_ret */);

extern CL_API_ENTRY cl_perf_monitor_qcom CL_API_CALL
clCreatePerfMonitorQCOM(cl_context                         /* context */,
                        cl_device_id                       /* device */,
                        cl_int                             /* num_counters */, 
                        cl_perf_monitor_counter_id_qcom * /* counter_list */,
                        cl_int *                           /* errcode_ret */);

extern CL_API_ENTRY cl_int CL_API_CALL
clRetainPerfMonitorQCOM(cl_perf_monitor_qcom /* monitor */);

extern CL_API_ENTRY cl_int CL_API_CALL
clReleasePerfMonitorQCOM(cl_perf_monitor_qcom /* monitor */);

extern CL_API_ENTRY cl_int CL_API_CALL
clEnqueueBeginPerfMonitorQCOM(cl_command_queue      /* command_queue */, 
                              cl_perf_monitor_qcom  /* monitor */, 
                              cl_uint               /* num_events_in_wait_list */,
                              const cl_event *      /* event_wait_list */, 
                              cl_event *            /* event */);

extern CL_API_ENTRY cl_int CL_API_CALL
clEnqueueEndPerfMonitorQCOM(cl_command_queue            /* command_queue */,
                                   cl_perf_monitor_qcom /* monitor */, 
                                   cl_uint              /* num_events_in_wait_list */, 
                                   const cl_event *     /* event_wait_list */, 
                                   cl_event *           /* event */);

extern CL_API_ENTRY cl_int CL_API_CALL 
clEnqueueReadPerfMonitorQCOM(cl_command_queue					 /* command_queue */, 
                                           cl_perf_monitor_qcom  /* monitor */,
                                           cl_bool               /* blocking_read */,
                                           void *                /* ptr */,
                                           cl_uint               /*  num_events_in_wait_list */,
                                           const cl_event *      /* event_wait_list */, 
                                           cl_event *            /* event */);

extern CL_API_ENTRY cl_int CL_API_CALL
clGetPerfMonitorInfoQCOM(cl_perf_monitor_qcom             /* monitor */, 
                         cl_perf_monitor_info_qcom        /* param_name */,
                         size_t                           /* param_value_size */,
                         void *                           /* param_value */,
                         size_t *                         /* param_value_size_ret */);
    
/* cl_device_info */
#define CL_DEVICE_MAX_PERF_MONITOR_GROUPS_QCOM     0xA000
#define CL_DEVICE_PERF_MONITOR_GROUP_LIST_QCOM     0xA001

/* cl_perf_monitor_group_info_qcom */
#define CL_PERF_MONITOR_MAX_COUNTERS_QCOM          0xA100
#define CL_PERF_MONITOR_MAX_ACTIVE_COUNTERS_QCOM   0xA101
#define CL_PERF_MONITOR_GROUP_NAME_QCOM            0xA102
#define CL_PERF_MONITOR_COUNTER_LIST_QCOM          0xA103

/* cl_perf_monitor_counter_info_qcom */
#define CL_PERF_MONITOR_COUNTER_NAME_QCOM          0xA200
#define CL_PERF_MONITOR_COUNTER_GROUP_QCOM         0xA201
#define CL_PERF_MONITOR_COUNTER_ID_QCOM			   0xA202
#define CL_PERF_MONITOR_COUNTER_TYPE_QCOM          0xA203
#define CL_PERF_MONITOR_COUNTER_RANGE_QCOM         0xA204   

        
/* cl_perf_monitor_counter_type */
#define CL_UNSIGNED_INT64_QCOM                     0xA300
#define CL_PERCENTAGE_QCOM                         0xA301
            
/* cl_perf_monitor_info_qcom */
#define CL_PERF_MONITOR_RESULT_SIZE_QCOM           0xA400
#define CL_PERF_MONITOR_CONTEXT_QCOM               0xA401
#define CL_PERF_MONITOR_REFERENCE_COUNT_QCOM       0xA402
        
/* New error codes */
#define CL_INVALID_PERF_MONITOR_QCOM               0xA500
#define CL_INVALID_PERF_MONITOR_GROUP_QCOM         0xA501
#define CL_INVALID_PERF_MONITOR_COUNTER_QCOM       0xA502
#define CL_PERF_MONITOR_RESULT_NOT_AVAILABLE       0xA503

/* cl_command_type */    
#define CL_COMMAND_BEGIN_PERF_MONITOR_QCOM         0xA600
#define CL_COMMAND_END_PERF_MONITOR_QCOM           0xA601
#define CL_COMMAND_READ_PERF_MONITOR_QCOM          0xA602


/*********************************
* cl_qcom_extended_queries_1 extension *                                                  
**********************************/

/* Accepted by clGetPlatformInfo */
#define CL_PLATFORM_NUM_OBJECTS_QCOM                     0xA700

/* Accepted by clGetDeviceInfo */
#define CL_DEVICE_NUM_32BIT_GPRS_PER_COMPUTE_UNIT_QCOM   0xA800
#define CL_DEVICE_NUM_WAVE_SIZES_QCOM                    0xA801
#define CL_DEVICE_SUPPORTED_WAVE_SIZES_QCOM              0xA802
#define CL_DEVICE_INSTRUCTION_STORAGE_SIZE_QCOM          0xA803

/* Accepted by clGetProgramBuildInfo */
#define CL_PROGRAM_WAVE_SIZE_QCOM                        0xA900
#define CL_PROGRAM_INSTRUCTION_SIZE_QCOM                 0xA901
#define CL_PROGRAM_DISASSEMBLY_QCOM                      0xA902

/* Accepted by clGetKernelWorkGroupInfo */
#define CL_KERNEL_NUM_32BIT_GPRS_PER_WORK_ITEM_QCOM      0xAA00
#define CL_KERNEL_STACK_SIZE_PER_WORK_ITEM_QCOM          0xAA01

/* Accepted by clGetKernelArgInfo */
#define CL_KERNEL_ARG_VALUE_QCOM                         0xAB00

#ifdef __cplusplus
}
#endif

#endif /* __OPENCL_CL_EXT_QCOM_H */
