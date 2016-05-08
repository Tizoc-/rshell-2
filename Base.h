#ifndef BASE_H
#define BASE_H
class Base {
    public:
        Base();
        virtual ~Base();
        virtual int execute() = 0;
};
#endif
