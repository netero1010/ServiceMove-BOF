# BOF - Lateral Movement Technique by Abusing DLL Hijacking on Windows Perception Simulation Service
New lateral movement technique by abusing Windows Perception Simulation Service to achieve DLL hijacking code execution.

**Limitation**

Windows 10 1809 or above only

**Usage**

===General use===  
Command: bof-servicemove target /root/hid.dll  

===Force mode===  
Description: restart the service if the service is already running  
Command: bof-servicemove target /root/hid.dll force

===Cleanup mode===  
Description: stop the service if running and delete the DLL payload file  
Command: bof-servicemove target cleanup

**Compile**

`make`

**Export Functions for "hid.dl"**  
Ref to "exports_function_hid.txt"

**Demo**

![HowTo](https://github.com/netero1010/ServiceMove-BOF/raw/main/demo.gif)
