

// PUBLIC VARIABLES
struct {
    short sw1;
    short sw2;
    short sw3;
    short sw4;
    short btn1;
    short btn2;
    short btn3;
} input_status;


// PUBLIC FUNTIONS

// update values of input_status
void update_inputs();

int getbtns(void);
int getsw(void);

// setup things for display, input etc
void setup_hardware();

// returns true if period has passed
short timer_ready();