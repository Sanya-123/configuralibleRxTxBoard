# configuralibleRxTxBoard
Example firmware that can configurate perepheria(spi, i2c, gpio) and function during work by send command on spi

Flixeble
  - function:add, remove, set auto call by timer
  - perepheria:init soft spi, init soft i2c(in future), init gpio, send data

Also configuration funtion and perepheria can be saved in memory
In firmware has hardware uart and cann be set handler of input data

## Function(handler)
In first start it is free firmware. You can build custom function(s) write in memory MCU above spi and then call this function(s) or set auto call
example function is in RxTxBoard_function with changed startup.s
function:
```C
int function(FunctionBoard *funct, char *argyment, uint32_t sizeArgument)
{
    (void)argyment;
    (void)sizeArgument;
    (void)funct;
    funct->readWriteInerface(1, 0x00d80005, 32);//off 0x00180005

    return 0;
}
```

## Perepheria
In first start init only hardware spi(control spi) and hardware uart.
On comming data on uart include some handler thats saved in memory like Function
You can add soft perepheria like spi, i2c(in future), gpio, also you can add cs for spi and add interface(spi + cs)
