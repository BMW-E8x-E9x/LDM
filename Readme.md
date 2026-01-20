I think it's entirely possible to apply gas/brakes using CANbus commands down to 0kph 
- I deduct that based on this post: https://www.e90post.com/forums/showthread.php?t=1530925&highlight=ldm&page=19#postcount28359314  and https://www.e90post.com/forums/showthread.php?t=1530925&highlight=ldm&page=15#td_post_27709475
- but I don't think the ACC vehicle option is even needed for gas/brake commands to work.  I always thought that with Dynamic Cruise Control it is the DSC module that controls the engine and brakes. Now I think it is actually a little LDM module (at least in 6cyl cars). 

Coding: https://www.e90post.com/forums/showthread.php?t=1530925&highlight=ldm&page=25#edit31117865

https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x0B6.md

http://share.qclt.com/bmw%E8%B5%84%E6%96%99/1102_Driving%20Dynamic%20Systems/English/Participant_manual/E90%20Driving%20Dynamics%20Systems.pdf

- LDM comes with 544 option only on 6-cylinder pre-2007 cars after that it will not be from factory but it is needed for ACC retrofit  
- also 544 option may come with xdrive cars and these car also can be 4-cylinder  and if 6-cylinder car in europe had'nt any cruise option at all it will require instrument cluster upgrade
- hupcheye flashed his 320i with $540 and let it control torque to speed target

### LDM control unit:
With optional equipment 544 (up to 09/06) and **optional equipment 541 in the E90/E91/E92**, the scope of functions of the software for cruise control is implemented in a separate control unit: the LDM control unit.

As of 09/06 or on the E92 with the series launch (06/06), the cruise control with braking function (optional extra 544) is integrated in the DSC control unit. The LDM control unit is therefore eliminated on optional extra 544 as of 09/06.

A precondition for the function is that the engine / transmission control and the DSC are operating without faults.

And the list of Can Msg. ID sent out from LDM module are :  
1) **0x0BF - 191: RequestedWheelTorqueDriveTrain -> DME**  
2) **0x0D5 - 213: Request_wheel_torque_brake -> DSC** 
3) 0x193 - 403: DynamicCruiseControlStatus -> KOMBI
4) 0X150 ?  336  Request_1_ACC -> ACC
5) 0x153 ?  339  Request_2_ACC -> ACC
6) ~~0x159 ?  345 ACC Object relative speed~~
7) ~~0x15C ?  348 ACC Object relative distance~~

5-series ACC requests:
Retard request ACC	ACC	0x0AD TBD, [partially decoded](https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x0AD.md)
Torque request ACC	ACC	0x0B7	[fully decoded](https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x0AD.md)
Display ACC	ACC	0x190	[TBD](https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x190.md)


That means my DME MEV1746 is not response to Msg. ID 0x193 not Msg. ID 0x0AA yet. And I believe Msg. ID 0x0AA is from DSC.  
If I translate/convert Msg. ID 0x193 to Msg. ID 0x200 which accepted by 4cyl BMW for basic cruise control, then only we can see whether Msg. ID 0x0AA controls works or not .... Let's find out !

![[Readme.md-20240809193828188.webp]]

## Coding
https://www.e90post.com/forums/showthread.php?t=1530925&highlight=dsc+pump&page=25#edit31117865
## Restore:

https://www.e90post.com/forums/showthread.php?t=1530925&highlight=ldm&page=13#edit27562083

![[Readme.md-20241107133338343.webp]]

![[Readme.md-20241107135030813.webp]]![[Readme.md-20241107140239075.webp]]
![[Readme.md-20241113233405741.webp]]

- Try `C0F_ECBA_DCC` to 0x3 to se if ACC brake will be listen in parallel.