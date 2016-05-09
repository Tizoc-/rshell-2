#ifndef BASE_H
#define BASE_H
class Base {
    //All subclasses will only inherit the execute function.
    //Any further variables will be added distinctly as seen fit.
    public:
        Base();
        virtual ~Base();
        virtual int execute() = 0;
};
#endif
