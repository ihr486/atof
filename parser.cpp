#include <stdio.h>
#include <stdlib.h>
#include <random>

static double conv_table[640];

void setup_conv_table(void)
{
    conv_table[320] = 1;
    for (int i = 1; i < 320; i++)
    {
        conv_table[320 + i] = conv_table[320 + i - 1] * 10;
    }
    for (int i = 1; i < 320; i++)
    {
        conv_table[320 - i] = conv_table[320 - i + 1] / 10;
    }
}

double parse_double(const char *str)
{
    bool signflag = false;
    if (*str == '-')
    {
        signflag = true;
        str++;
    }
    long int_part = 0;
    for (; '0' <= *str && *str <= '9'; str++)
    {
        int_part = int_part * 10 + (*str - '0');
    }
    long frac_part = 0, frac_digits = 0;
    if (*str == '.')
    {
        for(str++; '0' <= *str && *str <= '9'; str++)
        {
            frac_part = frac_part * 10 + (*str - '0');
            frac_digits++;
        }
    }
    long exponent = 0;
    if (*str == 'E' || *str == 'e')
    {
        bool expsign = false;
        str++;
        if (*str == '+')
        {
            str++;
        }
        else if (*str == '-')
        {
            expsign = true;
            str++;
        }
        for (; '0' <= *str && *str <= '9'; str++)
        {
            exponent = exponent * 10 + (*str - '0');
        }
        if (expsign) exponent = -exponent;
    }
    if (exponent <= -320 || 320 <= exponent) return 0;
    if (signflag) int_part = -int_part;
    return (double)int_part * conv_table[320 + exponent] + (double)frac_part * conv_table[320 + exponent - frac_digits];
}

int main(int argc, const char *argv[])
{
    std::mt19937 engine((unsigned int)time(NULL));
    std::uniform_real_distribution<double> dist(-16, +16);
    char buf[64];

    setup_conv_table();
    for (int i = 0; i < 100; i++)
    {
        sprintf(buf, "%le", pow(10, dist(engine)));
        double ref = atof(buf);
        double res = parse_double(buf);
        printf("Ref = %le, res = %le, err=%le\n", ref, res, res - ref);
    }

    return 0;
}
