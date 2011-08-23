/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacJoystick.cpp,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#include "MacJoystick.h"
#include "Engine.h"
#include "StringUtils.h"

// The code in this file is taken from the Apple Documentation for accessing HID devices.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/errno.h>
#include <sysexits.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>
#include <CoreFoundation/CFString.h>

void print_errmsg_if_io_err(int expr, const char* msg)
{
  do 
  {          
    IOReturn err = (expr);                              
    if (err != kIOReturnSuccess) 
    {                      
        fprintf(stderr, "%s - %s(%x,%d)\n", msg,         
                                mach_error_string(err), 
                                err, err & 0xffffff);   
        fflush(stderr);                                 
        exit(EX_OSERR);                                 
    }                                                   
  } while(0);
}                                              

void print_errmsg_if_err(int expr, const char* msg) 
{
  do 
  {             
    if (expr) {                                         
        fprintf(stderr, "%s\n", msg);                   
        fflush(stderr);                                 
        exit(EX_OSERR);                                 
    }                                                   
  } while(0);
}                                              


namespace Amju
{
static CFIndex gNestingLevel = 0;
static CFIndex gElementIndex = 0;

static IOHIDElementCookie gXAxisCookie = 0;
static IOHIDElementCookie gYAxisCookie = 0;
static IOHIDElementCookie gButton1Cookie = 0;
static IOHIDElementCookie gButton2Cookie = 0;
static IOHIDElementCookie gButton3Cookie = 0;

static IOHIDDeviceInterface **  theDevice = 0;


void MyTestQueues(IOHIDDeviceInterface **hidDeviceInterface);
void MyTestHIDDeviceInterface(IOHIDDeviceInterface **hidDeviceInterface);
void MyCreateHIDDeviceInterface(io_object_t hidDevice,
                            IOHIDDeviceInterface ***hidDeviceInterface);
void MyStoreImportantCookies(CFDictionaryRef element);
CFIndex MyGetElementIndex();
void MySetElementIndex(CFIndex newIndex);
void MyIncrementElementIndex(CFIndex increment);
void MyIndent();
void MyStartBlock(Boolean printIndex);
void MyEndBlock();
void MyCFStringShow(CFStringRef object);
void MyCFArrayShow(CFArrayRef object);
void MyCFArrayShowApplier(const void * value, void * parameter);
void MyCFBooleanShow(CFBooleanRef object);
void MyCFDictionaryShow(CFDictionaryRef object);
void MyCFNumberShow(CFNumberRef object);
void MyShowDictionaryElement(CFDictionaryRef dictionary,
                                    CFStringRef     key);
void MyShowProperty(const void * key, const void * value);
void MyCFObjectShow(CFTypeRef object);
void MyShowHIDProperties(io_registry_entry_t hidDevice);
void MyTestHIDDevices(io_iterator_t hidObjectIterator);
void MyFindHIDDevices(mach_port_t masterPort,
                io_iterator_t *hidObjectIterator);
void MyStartHIDDeviceInterfaceTest(void);                                                    
                            

void MyTestHIDInterface(IOHIDDeviceInterface ** hidDeviceInterface)
{
    HRESULT             result;
    IOHIDEventStruct    hidEvent;
    long                index;

    printf("X Axis (%d), Y Axis (%d), Button 1 (%d), Button 2 (%d), Button 3 (%d)\n",
            (long) gXAxisCookie, (long) gYAxisCookie, (long) gButton1Cookie, 
            (long) gButton2Cookie, (long) gButton3Cookie);

    for (index = 0; index < 2000; index++)
    {
        long xAxis, yAxis, button1, button2, button3;

        //Get x axis
        result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface,
                                            gXAxisCookie, &hidEvent);
        if (result) printf("getElementValue error = %lx", result);
        xAxis = hidEvent.value;

        // Get Y axis
        result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface,
                                            gYAxisCookie, &hidEvent);
        if (result) printf("getElementValue error = %lx", result);
        yAxis = hidEvent.value;

        //Get button 1
        result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface,
                                            gButton1Cookie, &hidEvent);
        if (result) printf("getElementValue error = %lx", result);
        button1 = hidEvent.value;

        //Get button 2
        result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface,
                                            gButton2Cookie, &hidEvent);
        if (result) printf("getElementValue error = %lx", result);
        button2 = hidEvent.value;

        //Get button 3
        result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface,
                                            gButton3Cookie, &hidEvent);
        if (result) printf("getElementValue error = %lx", result);
        button3 = hidEvent.value;

        //Print values
        printf("%ld %ld %s%s%s\n", xAxis, yAxis, button1 ? "button1 " : "",
                button2 ? "button2 " : "", button3 ? "button3 " : "");

        sleep(1);
    }
}
 
 

void MyTestQueues(IOHIDDeviceInterface **hidDeviceInterface)
{
    HRESULT                 result;
    IOHIDQueueInterface **  queue;
    Boolean                 hasElement;
    long                    index;
    IOHIDEventStruct        event;

    queue = (*hidDeviceInterface)->allocQueue(hidDeviceInterface);

    if (queue)
    {
        printf("Queue allocated: %lx\n", (long) queue);
        //create the queue
        result = (*queue)->create(queue,
                                 0,
                                 8);/* depth: maximum number of elements
                                    in queue before oldest elements in 
                                    queue begin to be lost*/
        printf("Queue create result: %lx\n", result);

        //add elements to the queue
        result = (*queue)->addElement(queue, gXAxisCookie, 0);
        printf("Queue added x axis result: %lx\n", result);
        result = (*queue)->addElement(queue, gButton1Cookie, 0);
        printf("Queue added button 1 result: %lx\n", result);
        result = (*queue)->addElement(queue, gButton2Cookie, 0);
        printf("Queue added button 2 result: %lx\n", result);
        result = (*queue)->addElement(queue, gButton3Cookie, 0);
        printf("Queue added button 3 result: %lx\n", result);


        //check to see if button 3 is in queue
        hasElement = (*queue)->hasElement(queue,gButton3Cookie);
        printf("Queue has button 3 result: %s\n", hasElement ? "true" : "false");

        //remove button 3 from queue
        result = (*queue)->removeElement(queue, gButton3Cookie);
        printf("Queue remove button 3 result: %lx\n",result);

        //start data delivery to queue
        result = (*queue)->start(queue);
        printf("Queue start result: %lx\n", result);

        //check queue a few times to see accumulated events
        sleep(1);
        printf("Checking queue\n");
        for (index = 0; index < 10; index++)
        {
            AbsoluteTime    zeroTime = {0,0};

            result = (*queue)->getNextEvent(queue, &event, zeroTime, 0);
            if (result)
                printf("Queue getNextEvent result: %lx\n", result);
            else
                printf("Queue: event:[%lx] %ld\n", 
                                (unsigned long) event.elementCookie,
                                event.value);

            sleep(1);
        }

        //stop data delivery to queue
        result = (*queue)->stop(queue);
        printf("Queue stop result: %lx\n", result);

        //dispose of queue
        result = (*queue)->dispose(queue);
        printf("Queue dispose result: %lx\n", result);

        //release the queue we allocated
        (*queue)->Release(queue);
    }
}


void MyTestHIDDeviceInterface(IOHIDDeviceInterface **hidDeviceInterface)
{
    IOReturn ioReturnValue = kIOReturnSuccess;

    //open the device
    ioReturnValue = (*hidDeviceInterface)->open(hidDeviceInterface, 0);
    printf("Open result = %d\n", ioReturnValue);

    //test queue interface
    //MyTestQueues(hidDeviceInterface);

    //test the interface
    MyTestHIDInterface(hidDeviceInterface);

    printf("Hello, closing the device.\n");

    //close the device
    if (ioReturnValue == KERN_SUCCESS)
        ioReturnValue = (*hidDeviceInterface)->close(hidDeviceInterface);

    printf("Hello, about to release the interface.\n");

    //release the interface
    (*hidDeviceInterface)->Release(hidDeviceInterface);
    
    printf("Well, that was ok.\n");
}


void MyCreateHIDDeviceInterface(io_object_t hidDevice,
                            IOHIDDeviceInterface ***hidDeviceInterface)
{
    io_name_t               className;
    IOCFPlugInInterface     **plugInInterface = NULL;
    HRESULT                 plugInResult = S_OK;
    SInt32                  score = 0;
    IOReturn                ioReturnValue = kIOReturnSuccess;

    ioReturnValue = IOObjectGetClass(hidDevice, className);

    print_errmsg_if_io_err(ioReturnValue, "Failed to get class name.");

    printf("Found device type %s\n", className);

    ioReturnValue = IOCreatePlugInInterfaceForService(hidDevice,
                            kIOHIDDeviceUserClientTypeID,
                            kIOCFPlugInInterfaceID,
                            &plugInInterface,
                            &score);

    if (ioReturnValue == kIOReturnSuccess)
    {
        //Call a method of the intermediate plug-in to create the device 
        //interface
        plugInResult = (*plugInInterface)->QueryInterface(plugInInterface,
                            CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID),
                            (void**) hidDeviceInterface);
        print_errmsg_if_err(plugInResult != S_OK, "CouldnÕt create HID class device interface");

        (*plugInInterface)->Release(plugInInterface);
    }
}
 
void MyStoreImportantCookies(CFDictionaryRef element)
{
    CFTypeRef           object;
    long                number;
    IOHIDElementCookie  cookie;
    long                usage;
    long                usagePage;

    //Get cookie
    object = (CFDictionaryGetValue(element, CFSTR(kIOHIDElementCookieKey)));
    if (object == 0 || CFGetTypeID(object) != CFNumberGetTypeID())
        return;
    if(!CFNumberGetValue((CFNumberRef) object, kCFNumberLongType, &number))
        return;
    cookie = (IOHIDElementCookie) number;

    //Get usage
    object = CFDictionaryGetValue(element, CFSTR(kIOHIDElementUsageKey));
    if (object == 0 || CFGetTypeID(object) != CFNumberGetTypeID())
        return;
    if (!CFNumberGetValue((CFNumberRef) object, kCFNumberLongType, &number))
        return;
    usage = number;

    //Get usage page
    object = CFDictionaryGetValue(element,CFSTR(kIOHIDElementUsagePageKey));
    if (object == 0 || CFGetTypeID(object) != CFNumberGetTypeID())
        return;
    if (!CFNumberGetValue((CFNumberRef) object, kCFNumberLongType, &number))
        return;
    usagePage = number;

    //Check for x axis
    if (usage == 0x30 && usagePage == 0x01)
        gXAxisCookie = cookie;
    // Check for Y axis - TODO TEMP TEST
    if (usage == 0x31 && usagePage == 0x01)
        gYAxisCookie = cookie;
    //Check for buttons
    else if (usage == 0x01 && usagePage == 0x09)
        gButton1Cookie = cookie;
    else if (usage == 0x02 && usagePage == 0x09)
        gButton2Cookie = cookie;
    else if (usage == 0x03 && usagePage == 0x09)
        gButton3Cookie = cookie;
}
 
 
CFIndex MyGetElementIndex()
{
    return gElementIndex;
}

void MySetElementIndex(CFIndex newIndex)
{
    gElementIndex = newIndex;
}

void MyIncrementElementIndex(CFIndex increment)
{
    gElementIndex += increment;
}

void MyIndent()
{
    int i;
    printf("\n");
    for (i = 0; i < gNestingLevel; i++) printf(" ");
}

void MyStartBlock(Boolean printIndex)
{
    if (printIndex)
    {
        MyIndent();
        printf("[%ld]", MyGetElementIndex());
    }
    gNestingLevel++;
}

void MyEndBlock()
{
    gNestingLevel--;
}

void MyCFStringShow(CFStringRef object)
{
    const char * c = CFStringGetCStringPtr(object,
                                        CFStringGetSystemEncoding());
    if (c)
    {
        printf("%s", c);
    }
    else
    {
        CFIndex bufferSize = CFStringGetLength(object) + 1;
        char * buffer = (char *)malloc(bufferSize);

        if (buffer)
            {
                if (CFStringGetCString(object,
                                        buffer,
                                        bufferSize,
                                        CFStringGetSystemEncoding()))
                {
                    printf("%s", buffer);
                }
                free(buffer);
            }
    }
}

void MyCFArrayShow(CFArrayRef object)
{
    CFRange range = {0, CFArrayGetCount(object)};
    CFIndex savedIndex = MyGetElementIndex();

    //Show an element array containing one or more element dictionaries
    MySetElementIndex(0);   //Reset index to zero
    MyStartBlock(FALSE);
    CFArrayApplyFunction(object, range, MyCFArrayShowApplier, 0);
    MyEndBlock();

    MySetElementIndex(savedIndex);  //Restore original index
}

void MyCFArrayShowApplier(const void * value, void * parameter)
{
    if (CFGetTypeID(value) != CFDictionaryGetTypeID()) return;

    MyCFObjectShow(value);
}

void MyCFBooleanShow(CFBooleanRef object)
{
    printf(CFBooleanGetValue(object) ? "true" : "false");
}

void MyCFDictionaryShow(CFDictionaryRef object)
{
    MyStartBlock(TRUE);

    MyShowDictionaryElement(object, CFSTR(kIOHIDElementCookieKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementTypeKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementCollectionTypeKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementUsageKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementUsagePageKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementMinKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementMaxKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementScaledMinKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementScaledMaxKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementSizeKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementIsRelativeKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementIsWrappingKey));
//    MyShowDictionaryElement(object, CFSTR(kIOHIDElementInNonLinearKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementHasPreferredStateKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementHasNullStateKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementVendorSpecificKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementUnitKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementUnitExponentKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementNameKey));
    MyShowDictionaryElement(object, CFSTR(kIOHIDElementKey));

    MyStoreImportantCookies(object);    //Store specific elements in global
                                        //variables for use in queue
                                        //handling functions.
    MyEndBlock();
    MyIncrementElementIndex(1);
}

void MyCFNumberShow(CFNumberRef object)
{
    long number;

    if (CFNumberGetValue(object, kCFNumberLongType, &number));
    {
        printf("0x%lx (%ld)", number, number);
    }
}

void MyShowDictionaryElement(CFDictionaryRef dictionary,
                                    CFStringRef     key)
{
    CFTypeRef object = CFDictionaryGetValue(dictionary, key);

    if (object)
        MyShowProperty(key,object);
}

void MyShowProperty(const void * key, const void * value)
{
    //Both parameters are references to CF objects
    MyIndent();
    MyCFStringShow((CFStringRef)key);
    printf(" = ");
    MyCFObjectShow(value);
}

void MyCFObjectShow(CFTypeRef object)
{
    CFTypeID type = CFGetTypeID(object);

    if      (type == CFArrayGetTypeID()) MyCFArrayShow((CFArrayRef)object);
    else if (type == CFBooleanGetTypeID()) MyCFBooleanShow((CFBooleanRef)object);
    else if (type == CFDictionaryGetTypeID()) MyCFDictionaryShow((CFDictionaryRef)object);
    else if (type == CFNumberGetTypeID()) MyCFNumberShow((CFNumberRef)object);
    else if (type == CFStringGetTypeID()) MyCFStringShow((CFStringRef)object);
    else printf("<unknown object>\n");
}


void MyShowHIDProperties(io_registry_entry_t hidDevice)
{
    kern_return_t           result;
    CFMutableDictionaryRef  properties = 0;
    char                    path[512];

    result = IORegistryEntryGetPath(hidDevice,
                                    kIOServicePlane,
                                    path);
    if (result == KERN_SUCCESS)
        printf("[ %s ]", path);

    //Create a CF dictionary representation of the I/O 
    //Registry entryÕs properties
    result = IORegistryEntryCreateCFProperties(hidDevice,
                                            &properties,
                                            kCFAllocatorDefault,
                                            kNilOptions);
    if ((result == KERN_SUCCESS) && properties)
    {
        MyShowDictionaryElement(properties, CFSTR(kIOHIDTransportKey));
//        MyShowDictionaryElement(properties, CFSTR(kIOHIDVendorKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDProductIDKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDVersionNumberKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDManufacturerKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDProductKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDSerialNumberKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDLocationIDKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDPrimaryUsageKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDPrimaryUsagePageKey));
        MyShowDictionaryElement(properties, CFSTR(kIOHIDElementKey));

        //Release the properties dictionary
        CFRelease(properties);
    }
    printf("\n\n");
}


void MyTestHIDDevices(io_iterator_t hidObjectIterator)
{
    io_object_t             hidDevice = NULL;
    IOHIDDeviceInterface    **hidDeviceInterface = NULL;
    IOReturn                ioReturnValue = kIOReturnSuccess;

    while ((hidDevice = IOIteratorNext(hidObjectIterator)))
    {
        MyShowHIDProperties(hidDevice);
        MyCreateHIDDeviceInterface(hidDevice, &hidDeviceInterface);
        ioReturnValue = IOObjectRelease(hidDevice);
        print_errmsg_if_io_err(ioReturnValue, "Error releasing HID device");
        if (hidDeviceInterface != NULL)
        {
            MyTestHIDDeviceInterface(hidDeviceInterface);
            
            printf("About to release device interface.\n");
            //(*hidDeviceInterface)->Release(hidDeviceInterface);
            printf("..released it.\n");
        }
    }
    IOObjectRelease(hidObjectIterator);
}


void MyFindHIDDevices(mach_port_t masterPort,
                io_iterator_t *hidObjectIterator)
{
    CFMutableDictionaryRef hidMatchDictionary = NULL;
    IOReturn ioReturnValue = kIOReturnSuccess;
    Boolean noMatchingDevices = false;

    // Set up a matching dictionary to search the I/O Registry by class
    // name for all HID class devices
    hidMatchDictionary = IOServiceMatching(kIOHIDDeviceKey);

    // Now search I/O Registry for matching devices.
    ioReturnValue = IOServiceGetMatchingServices(masterPort, 
                            hidMatchDictionary, hidObjectIterator);

    noMatchingDevices = ((ioReturnValue != kIOReturnSuccess) 
                                | (*hidObjectIterator == NULL));

    //If search is unsuccessful, print message and hang.
    if (noMatchingDevices)
        print_errmsg_if_err(ioReturnValue, "No matching HID class devices found.");

    // IOServiceGetMatchingServices consumes a reference to the
    //   dictionary, so we don't need to release the dictionary ref.
    hidMatchDictionary = NULL;
}


 
void MyStartHIDDeviceInterfaceTest(void)
{
    mach_port_t masterPort = NULL;
    io_iterator_t hidObjectIterator = NULL;
    IOReturn ioReturnValue = kIOReturnSuccess;

    //Get a Mach port to initiate communication with I/O Kit.
    ioReturnValue = IOMasterPort(bootstrap_port, &masterPort);

    // If error, print message and hang (for debugging purposes)
    print_errmsg_if_io_err(ioReturnValue, "CouldnÕt create a master I/O Kit Port.");

    MyFindHIDDevices(masterPort, &hidObjectIterator);

    if (hidObjectIterator != NULL)
    {
        MyTestHIDDevices(hidObjectIterator);

        //Release iterator. DonÕt need to release iterator objects.
        IOObjectRelease(hidObjectIterator);
    }

    //Free master port if we created one.
    if (masterPort)
        mach_port_deallocate(mach_task_self(), masterPort);
}




///////////////////////////////////////////////////////////////////////////////////////////


void jcPollDevice(IOHIDDeviceInterface ** hidDeviceInterface)
{
    HRESULT             result;
    IOHIDEventStruct    hidEvent;
    long                index;

    //printf("X Axis (%d), Y Axis (%d), Button 1 (%d), Button 2 (%d), Button 3 (%d)\n",
    //        (long) gXAxisCookie, (long) gYAxisCookie, (long) gButton1Cookie, 
    //        (long) gButton2Cookie, (long) gButton3Cookie);

    long xAxis, yAxis, button1, button2, button3;

    //Get x axis
    result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface, gXAxisCookie, &hidEvent);
    if (result) printf("getElementValue error = %lx\n", result);
    xAxis = hidEvent.value;

    // Get Y axis
    result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface, gYAxisCookie, &hidEvent);
    yAxis = hidEvent.value;

    //Get button 1
    result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface, gButton1Cookie, &hidEvent);
    if (result) printf("getElementValue error = %lx\n", result);
    button1 = hidEvent.value;

    //Get button 2
    result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface, gButton2Cookie, &hidEvent);
    if (result) printf("getElementValue error = %lx\n", result);
    button2 = hidEvent.value;

    //Get button 3
    //result = (*hidDeviceInterface)->getElementValue(hidDeviceInterface, gButton3Cookie, &hidEvent);
    //if (result) printf("getElementValue error = %lx\n", result);
    //button3 = hidEvent.value;

    //Print values
    //printf("%ld %ld %s%s%s\n", xAxis, yAxis, button1 ? "button1 " : "",
    //        button2 ? "button2 " : "", button3 ? "button3 " : "");


    float fx = ((float)xAxis - 127.0f) / 128.0f;
    Engine::Instance()->JoyX(fx);
    
    float fy = ((float)yAxis - 127.0f) / 128.0f;
    Engine::Instance()->JoyY(-fy);

    static bool blue = false;
    bool newBlue = (button1 != 0);
    if (newBlue != blue)
    {
      Engine::Instance()->Blue(newBlue);
      blue = newBlue;
    }
    
    static bool z = false;
    bool newZ = (button2 != 0);
    if (newZ != z)
    {
      Engine::Instance()->Z(newZ);
      z = newZ;
    }
}


void jcTestHIDDeviceInterface(IOHIDDeviceInterface **hidDeviceInterface)
{
    IOReturn ioReturnValue = kIOReturnSuccess;

    //open the device
    ioReturnValue = (*hidDeviceInterface)->open(hidDeviceInterface, 0);
    printf("Open result = %d\n", ioReturnValue);

    //test queue interface
    //MyTestQueues(hidDeviceInterface);

    //test the interface
    //jcPollDevice(hidDeviceInterface);

    //printf("Hello, closing the device.\n");

    //close the device
    //if (ioReturnValue == KERN_SUCCESS)
    //    ioReturnValue = (*hidDeviceInterface)->close(hidDeviceInterface);

    //printf("Hello, about to release the interface.\n");

    //release the interface
    //(*hidDeviceInterface)->Release(hidDeviceInterface);
    
    //printf("Well, that was ok.\n");
}



void jcStoreDeviceList(io_iterator_t hidObjectIterator)
{
    // Iterate over the deives. Get the name of each one. 
    // Then the user can choose which device to use.
    io_object_t             hidDevice = NULL;
    IOHIDDeviceInterface    **hidDeviceInterface = NULL;
    IOReturn                ioReturnValue = kIOReturnSuccess;

    while ((hidDevice = IOIteratorNext(hidObjectIterator)))
    {
        MyShowHIDProperties(hidDevice);
        MyCreateHIDDeviceInterface(hidDevice, &hidDeviceInterface);
        ioReturnValue = IOObjectRelease(hidDevice);
        print_errmsg_if_io_err(ioReturnValue, "Error releasing HID device");
        if (hidDeviceInterface != NULL)
        {
            // Get the device name. We don't want the keyboard or mouse!
            // CFMutableDictionaryRef  properties = 0;
            char path[512];
            kern_return_t result = IORegistryEntryGetPath(hidDevice,
                                    kIOServicePlane,
                                    path);
            if (result != KERN_SUCCESS)
            {
              continue;
            }
            printf("[ %s ]\n", path);
            // If this device is NOT a keyboard and is NOT a mouse, we'll assume it's a joystick.
            std::string strPath(Amju::ToLower(std::string(path)));
            
            if (!Amju::StringContains(strPath, "mouse") && !Amju::StringContains(strPath, "keyboard"))
            {
              printf("THIS IS THE JOYSTICK, hopefully...\n");
              jcTestHIDDeviceInterface(hidDeviceInterface);
              theDevice = hidDeviceInterface;
              return;
            }           
            //printf("About to release device interface.\n");
            //(*hidDeviceInterface)->Release(hidDeviceInterface);
            //printf("..released it.\n");
        }
    }
    IOObjectRelease(hidObjectIterator);   
}


bool MacJoystick::IsConnected() const 
{ 
  return (theDevice); 
}

void MacJoystick::Poll(Engine* e) 
{
  if (e->IsJoystickEnabled() && theDevice)
  {
    jcPollDevice(theDevice);
  }
  
}

bool MacJoystick::Connect(int) 
{ 

    mach_port_t masterPort = NULL;
    io_iterator_t hidObjectIterator = NULL;
    IOReturn ioReturnValue = kIOReturnSuccess;

    //Get a Mach port to initiate communication with I/O Kit.
    ioReturnValue = IOMasterPort(bootstrap_port, &masterPort);

    MyFindHIDDevices(masterPort, &hidObjectIterator);

    if (hidObjectIterator == NULL)
    {
        return false;
    }
    
    jcStoreDeviceList(hidObjectIterator);

    //MyStartHIDDeviceInterfaceTest();
  
    return (theDevice); 
}

void MacJoystick::SetSensitivity(int) 
{
}
}
