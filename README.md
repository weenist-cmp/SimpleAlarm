# SimpleAlarm  

SimpleAlarm is Simple !!  

- Screen  
minute ... second / 60 <= Alarm  
hour:minute:second <= current time  

- Usage  
SimpleAlarm.exe Time Modify Min Max WavSoundFileName  
Time>0, Modify>0, Min>0, Min<=Max  
all value second  
ex1) 10 2 1 60  
ex2) 3600 3600 3600 36000  
ex3) 1 1 1 100 "C:\My Folder\My Best Lovely Song.wav"  
don't forger add "path file name" when path & filename with blank space  

- Default Value  
600 300 300 36000 Simple_drum2.wav  
Make Shortcut File  

- Keyboard  
UP: Time + Modify  
DOWN: Time - Modify  
SPACE / RETURN: StartWait  
ESCAPE : App Exit  

- StartWait  
App MINIMIZE  
Start Wait While Alarm Time Second  
App Restore  
Play WavSoundFile (once: screensaver is NOT active, repeat: Active, Nosound: File not found)  

- StopWait  
When App RESTORE  
Stop Wait (Beep)  

- Change  
0.4:  Monitor Turn On, Always Repeat Wav, Stop Play when Key  

- Etc.  
http://weenist.egloos.com/6532989  
2019.09.30.Mon.  
2019.10.03.Thu.  
Produce by weenist.  
