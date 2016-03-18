#ifndef __OPTION_H__
#define __OPTION_H__

#include <string>

namespace blobdetective {

/**
 * Type tags for Option values.
 */
typedef enum
{
    INVALID = 0,
    BOOLEAN = 'b',
    FLOAT = 'f',
    INT = 'i',
    STRING = 's',
} OptionType;

class Option
{
    public:
        Option(OptionType type, const char* name);
        bool is_type(OptionType type) const;
        const std::string& get_name() const;

        bool get_boolean() const;
        float get_float() const;
        int get_int() const;
        const std::string& get_string() const;

        void set_boolean(bool value);
        void set_float(float value);
        void set_int(int value);
        void set_string(const char* value);

        bool is_string() const;
        bool is_float() const;
        bool is_int() const;
        bool is_boolean() const;

    private:
        OptionType _type;
        std::string _name;
        int _int;
        float _float;
        std::string _string;
        bool _boolean;
};

} // end of namespace

#endif // __OPTION_H__
