androidcl
=======

Sample app that runs OpenCL kernels to process camera feed on Android devices


File Structure
--------------

 * assets (contains the opencl kernels source file)
 * jni (JNI source code that helps run opencl kernels and the necessary native c/c++ code)
 * src (java source files)


How to build the native code?
-----------------------------

Run the command `ndk-build` from android application project root folder. If you found any issues with code, you are welcome to submit a fix or let us know so that we can fix it. Thank you.


How to build java code?
-----------------------

If you are aware of ant build tool, run the command `ant debug install` with your testing mobile device attached to your development machine. That should do a debug build and install it on to your testing device.

If you are not aware of `ant`, you can export this entire droidcl repository as an application in eclipse that has the capability to develop android applications. Rest of it is similar to how you build any other app on eclipse.
