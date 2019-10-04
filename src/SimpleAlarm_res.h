#ifndef SimpleAlarm_res_H
#define SimpleAlarm_res_H

#define V_Major   0
#define V_Minor   3
#define V_Release 1
#define V_Build   1

#define _STR(n) #n
#define STR(n) _STR(n)

#define V_String STR(V_Major) "." STR(V_Minor) "." STR(V_Release) "." STR(V_Build)

#define P_Name        "SimpleAlarm"
#define F_Description "SimpleAlarm is Simple !!"
#define P_FileName    P_Name ".exe"

#define P_Copyright   "weenist"

#define ID_tInfo      1001
#define ID_bStart     1002

#endif
