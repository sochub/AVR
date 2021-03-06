#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
    template<uint8_t nr>
    struct ExternalInterruptTemplate
    {
        static const uint8_t baseNumber = 1;

        enum Interrupts
        {
            externalInterrupt = baseNumber + nr,
        };

        typedef avr_halib::interrupts::interrupt_manager::Slot< externalInterrupt,
            avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction > ExternalInterruptSlot;
        typedef typename boost::mpl::vector< ExternalInterruptSlot >::type Slots;
    };

    typedef ExternalInterruptTemplate<0> ExternalInterrupt0;
    typedef ExternalInterruptTemplate<1> ExternalInterrupt1;
    typedef ExternalInterruptTemplate<2> ExternalInterrupt2;
    typedef ExternalInterruptTemplate<3> ExternalInterrupt3;
    typedef ExternalInterruptTemplate<4> ExternalInterrupt4;
    typedef ExternalInterruptTemplate<5> ExternalInterrupt5;
    typedef ExternalInterruptTemplate<6> ExternalInterrupt6;
    typedef ExternalInterruptTemplate<7> ExternalInterrupt7;
}
}
}
