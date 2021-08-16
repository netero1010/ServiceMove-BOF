# BOF - Lateral Movement Technique by Abusing DLL Hijacking on Windows Perception Simulation Service
----
ServiceMove is a POC code for an interesting lateral movement technique by abusing Windows Perception Simulation Service to achieve DLL hijacking code execution.

A non-existing DLL file (i.e., hid.dll) will be loaded everytime when "Windows Perception Simulation Service" was started. By inserting a crafted DLL in "C:\Windows\System32\PerceptionSimulation" and starting the service remotely, we were able to achieve code execution as "NT AUTHORITY\SYSTEM" in a remote system.

The beauty of this technique is that it is relatively sleathy/OPSEC since it doesn't have the typical IOCs like other general lateral movement techniques (e.g., service creation/modification, scheduled task creation). All it will do is just dropping a file to remote system and starting a service remotely.

### Limitation

Windows 10 1809 or above only

### Common Line Usage
```
Version: 1.0
Author: Chris Au
Twitter: @netero_1010
Github: @netero1010

===General use===  
Command: bof-servicemove target /root/hid.dll  

===Force mode===  
Description: restart the service if the service is already running  
Command: bof-servicemove target /root/hid.dll force

===Cleanup mode===  
Description: stop the service if running and delete the DLL payload file  
Command: bof-servicemove target cleanup
```

### Compile

`make`

**Export Functions for "hid.dl"**  
Ref to [exports_function_hid.txt](https://github.com/netero1010/ServiceMove-BOF/blob/main/exports_function_hid.txt)

### Demo

![HowTo](https://github.com/netero1010/ServiceMove-BOF/raw/main/demo.gif)
