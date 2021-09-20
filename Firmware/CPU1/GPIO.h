#ifndef GPIO_H_
#define GPIO_H_

    void InitButtons(void);
    void InitSwitches(void);
    void InitLEDs(void);

    Uint16 get_buttons(void);
    Uint16 get_switches(void);
    void set_LEDs(Uint16 data);

#endif
