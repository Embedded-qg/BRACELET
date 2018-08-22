#ifndef __MAX30102_MAIN_H
#define __MAX30102_MAIN_H

#define   aver_num    10                //定义多少个数据取一次平均

/*函数的声明*/
void max30102_main(void);  
float get_aver_spO2(signed int SP02[]);
signed int get_aver_hr(signed int heart_rate[]);

#endif
