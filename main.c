
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793
#define CLEAR_SCREEN "\033[2J\033[H"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define WHITE   "\033[1;37m"

void line()
{
    printf(CYAN);
    printf("============================================================\n");
    printf(RESET);
}

void heading()
{
    printf(CLEAR_SCREEN);

    line();
    printf(MAGENTA);
    printf("              ELECTRICAL ENGINEERING PROJECT\n");
    printf(RESET);

    printf(CYAN);
    printf("              SINGLE PHASE RLC CIRCUIT ANALYZER\n");
    printf(RESET);

    line();

    printf(YELLOW);
    printf("  Analyze | Calculate | Understand | Explore\n");
    printf(RESET);

    line();

    printf("\n");
    printf(WHITE);
    printf("  This program calculates the electrical parameters\n");
    printf("  of a SERIES RLC circuit using AC supply.\n");
    printf(RESET);

    printf("\n");
}

double getInput(char message[])
{
    double value;

    while (1)
    {
        printf(CYAN "  %-35s : " RESET, message);

        if (scanf("%lf", &value) != 1)
        {
            printf(RED "\n  Invalid input! Enter a number.\n" RESET);

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        if (!isfinite(value))
        {
            printf(RED "  Enter a finite number.\n" RESET);
            continue;
        }

        return value;
    }
}

int main()
{
    double V, f, R, L, C;

    double XL, XC, Z;
    double I, PF, angle;

    double P, Q, S;
    double VR, VL, VC;

    double resonanceFrequency;
    double reactiveDifference;

    heading();

    // INPUT SECTION

    printf(BLUE "  [1] ENTER CIRCUIT PARAMETERS\n\n" RESET);

    do
    {
        V = getInput("Supply Voltage (Volts RMS)");
        if (V <= 0)
            printf(RED "  Voltage must be positive.\n" RESET);
    } while (V <= 0);

    do
    {
        f = getInput("Supply Frequency (Hz)");
        if (f <= 0)
            printf(RED "  Frequency must be positive.\n" RESET);
    } while (f <= 0);

    do
    {
        R = getInput("Resistance (Ohms)");
        if (R <= 0)
            printf(RED "  Resistance must be positive.\n" RESET);
    } while (R <= 0);

    do
    {
        L = getInput("Inductance (mH)");
        if (L <= 0)
            printf(RED "  Inductance must be positive.\n" RESET);
    } while (L <= 0);

    do
    {
        C = getInput("Capacitance (uF)");
        if (C <= 0)
            printf(RED "  Capacitance must be positive.\n" RESET);
    } while (C <= 0);

    // UNIT CONVERSION

    L = L / 1000.0;
    C = C / 1000000.0;

    // CALCULATION SECTION

    XL = 2 * PI * f * L;

    XC = 1 / (2 * PI * f * C);

    reactiveDifference = XL - XC;

    Z = sqrt(R * R +
             reactiveDifference * reactiveDifference);

    I = V / Z;

    PF = R / Z;

    angle = atan2(reactiveDifference, R) * 180 / PI;

    // POWER CALCULATIONS

    P = I * I * R;

    Q = I * I * reactiveDifference;

    S = V * I;

    // VOLTAGE DROPS

    VR = I * R;
    VL = I * XL;
    VC = I * XC;

    // RESONANCE FREQUENCY

    resonanceFrequency =
        1 / (2 * PI * sqrt(L * C));

    // OUTPUT SECTION

    printf("\n");
    line();

    printf(GREEN);
    printf("                 CALCULATION RESULTS\n");
    printf(RESET);

    line();

    printf("\n" YELLOW "  [2] REACTANCE VALUES\n" RESET);

    printf("  Inductive Reactance (XL)      : %10.3f Ohms\n", XL);
    printf("  Capacitive Reactance (XC)     : %10.3f Ohms\n", XC);
    printf("  Net Reactance (XL - XC)       : %10.3f Ohms\n", reactiveDifference);

    printf("\n" YELLOW "  [3] CIRCUIT PARAMETERS\n" RESET);

    printf("  Total Impedance (Z)           : %10.3f Ohms\n", Z);
    printf("  Circuit Current (I)           : %10.3f A\n", I);
    printf("  Power Factor                  : %10.4f\n", PF);
    printf("  Phase Angle                   : %10.3f Degrees\n", angle);

    printf("\n" YELLOW "  [4] VOLTAGE DISTRIBUTION\n" RESET);

    printf("  Resistor Voltage (VR)         : %10.3f V\n", VR);
    printf("  Inductor Voltage (VL)         : %10.3f V\n", VL);
    printf("  Capacitor Voltage (VC)        : %10.3f V\n", VC);

    printf("\n" YELLOW "  [5] POWER ANALYSIS\n" RESET);

    printf("  Active Power (P)              : %10.3f W\n", P);
    printf("  Reactive Power (Q)            : %10.3f VAR\n", Q);
    printf("  Apparent Power (S)            : %10.3f VA\n", S);

    printf("\n" YELLOW "  [6] RESONANCE ANALYSIS\n" RESET);

    printf("  Resonant Frequency            : %10.3f Hz\n",
           resonanceFrequency);

    printf("  Supply Frequency              : %10.3f Hz\n", f);

    // CIRCUIT CONDITION

    printf("\n");
    line();

    printf(MAGENTA);
    printf("                  CIRCUIT ANALYSIS\n");
    printf(RESET);

    line();

    if (fabs(reactiveDifference) < 1e-10)
    {
        printf(GREEN "  CONDITION: SERIES RESONANCE\n" RESET);
        printf("  XL and XC are approximately equal.\n");
        printf("  Power factor is unity.\n");
    }
    else if (reactiveDifference > 0)
    {
        printf(YELLOW "  CONDITION: INDUCTIVE CIRCUIT\n" RESET);
        printf("  Current lags the supply voltage.\n");
        printf("  Power factor is LAGGING.\n");
    }
    else
    {
        printf(BLUE "  CONDITION: CAPACITIVE CIRCUIT\n" RESET);
        printf("  Current leads the supply voltage.\n");
        printf("  Power factor is LEADING.\n");
    }

    printf("\n");

    if (f > resonanceFrequency)
        printf("  Operating above resonant frequency.\n");
    else if (f < resonanceFrequency)
        printf("  Operating below resonant frequency.\n");
    else
        printf("  Operating at resonant frequency.\n");

    line();

    printf(GREEN);
    printf("            ANALYSIS COMPLETED SUCCESSFULLY!\n");
    printf(RESET);

    printf(CYAN);
    printf("              Thank you for using this tool!\n");
    printf(RESET);

    line();

    return 0;
}
