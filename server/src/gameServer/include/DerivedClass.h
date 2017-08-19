#ifndef DERIVEDE_CLASS_H
#define DERIVEDE_CLASS_H

#include "myAppLib.h"

class DerivedClass : public MyAppLib
{
    public:
                 DerivedClass(void);
        virtual ~DerivedClass(void);

        void     print(void) override;
};

#endif // DERIVEDE_CLASS_H
