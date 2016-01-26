#include "soapcalcProxy.h"
#include "calc.nsmap"
int main()
{
    calcProxy service;
    double result;
    if (service.add(4.0, 2.0, result) == SOAP_OK)
        std::cout << "The sum of 4.0 and 2.0 is " << result << std::endl;
    else
        service.soap_stream_fault(std::cerr);
    service.destroy(); // delete data and release memory
} 
