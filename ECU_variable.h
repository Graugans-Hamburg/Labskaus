#ifndef ECU_VARIABLE_H
#define ECU_VARIABLE_H
#include <string>

enum EnumDataType
{
    type_unknown,
    type_u8,
    type_i8,
    type_u16,
    type_i16,
    type_i32,
    type_u32,
    type_f32,
    type_f64

};

class ECU_variable
{
    public:
        ECU_variable();
        virtual ~ECU_variable();
        unsigned int GetAddress() { return Address; }
        void SetAddress(unsigned int val) { Address = val; }
        EnumDataType GetDataType() { return Datatype; }
        void SetDataType(EnumDataType val) { Datatype = val; }
        std::string GetName() { return Name; }
        void SetName(std::string val) { Name = val; }
        std::string GetDescription() { return Description; }
        void SetDescription(std::string val) { Description = val; }
        void ParseDatatyp(const char *);
        std::string GetDatatypAsString(void);
    protected:
    private:
        unsigned int Address;
        EnumDataType Datatype;
        std::string Name;
        std::string Description;
};

#endif // ECU_VARIABLE_H
