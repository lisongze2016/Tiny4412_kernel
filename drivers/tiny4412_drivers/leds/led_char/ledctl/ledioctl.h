#ifndef MYLED_H_
#define MYLED_H_

#define LEDTYPE 'L'

#define LED_STAT 	_IOR(LEDTYPE, 0, int)
#define LED_CONTRL 	_IOW(LEDTYPE, 1, int)


#endif  //ioctl to control led
