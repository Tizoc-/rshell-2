#ifndef TEST_H
#define TEST_H
class Test {
    private:
        std::vector<std:string> commandVec;
    public:
        Test(std::vector<std::string> commandVec);
        int execute();
};
#endif