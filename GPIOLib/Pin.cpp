#include "Pin.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cassert>
 
namespace gpio {

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)
 
 
// I/O access
volatile unsigned *gpio;
 
 
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
 
#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0
 
#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH
 
#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

//---------------------------------------------------------------------


Pin::Pin(int pin, Direction dir)
: _pin(pin)
, _direction(dir)
{
    _pin = pin;
    _direction = dir;

    /* open /dev/mem */
    auto file = open("/dev/mem", O_RDWR|O_SYNC);
   if (file < 0) {
      printf("can't open /dev/mem \n");
      return;
   }
 
   /* mmap GPIO */
   void *gpio_map;
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      file,             //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );
 
   close(file); //No need to keep file open after mmap
 
   if (gpio_map == MAP_FAILED) {
      // printf("mmap error %d\n", (int)gpio_map);//errno also set!
      printf("mmap error");//errno also set!
      exit(-1);
   }
 
   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


   INP_GPIO(pin); // must use INP_GPIO before we can use OUT_GPIO

    if (dir == Output) 
    {
        OUT_GPIO(pin);
        set(false);
    }
}

Pin::~Pin()
{}

//---------------------------------------------------------------------

auto Pin::direction() const -> Pin::Direction { return _direction; }
auto Pin::pin()       const -> int            { return _pin; }

//---------------------------------------------------------------------

void Pin::set(bool value)
{
    assert( _direction == Output );

    if (value)
        GPIO_SET = 1 << _pin;
    else
        GPIO_CLR = 1 << _pin;
}

//---------------------------------------------------------------------

bool Pin::get()
{
    assert( _direction == Input );

    return (GET_GPIO(_pin) != 0);
}

}

