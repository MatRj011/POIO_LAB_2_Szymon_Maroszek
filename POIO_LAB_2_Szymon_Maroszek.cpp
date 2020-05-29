#include <iostream>
#include <fstream>
#include <math.h>
#include <array>
#include "ffft/FFTReal.h"
using namespace std;

// ----- LAB_1 ----- //

class Packet
{
protected:
    string device;
    string description;
    long date;

public:
    virtual void toString()
    {
        cout << "device: " << device << " description: " << description << " date: " << date << endl;
    }

    Packet(string d = "abc", string de = "cba", long da = 11)
    {
        device = d;
        description = de;
        date = da;
    }
};

template <class T, int I = 5>
class Sequence : public Packet
{
protected:
    int channelNr;
    string unit;
    double resolution;
    T tab[I];

public:
    virtual void toString()
    {
        Packet::toString();
        cout << "channelNr: " << channelNr << " unit: " << unit << " resolution: " << resolution << endl;
    }

    Sequence(string d = "abc", string de = "cba", long da = 11, int ch = 1, string u = "Hz", double r = 29.3) : Packet(d, de, da)
    {
        channelNr = ch;
        unit = u;
        resolution = r;
    }
};

class TimeHistory : public Sequence<char, 5>
{
    double sensitivity;

public:
    void toString()
    {
        Sequence::toString();
        cout << "sensitivity: " << sensitivity << endl;
    }

    TimeHistory(string d = "abc", string de = "cba", long da = 11, int ch = 1, string u = "Hz", double r = 29.3, double s = 99.34) : Sequence(d, de, da, ch, u, r)
    {
        sensitivity = s;
    }
};

class Spectrum : public Sequence<char, 5>
{
    double scaling;

public:
    void toString()
    {
        Sequence::toString();
        cout << "scaling: " << scaling << endl;
    }

    Spectrum(string d = "abc", string de = "cba", long da = 11, int ch = 1, string u = "Hz", double r = 29.3, double s = 12.1) : Sequence(d, de, da, ch, u, r)
    {
        scaling = s;
    }
};

class Alarm : public Packet
{
    int channelNr;
    int threshold;
    int direction;

public:
    void toString()
    {
        Packet::toString();
        cout << "threshold: " << threshold << " direction: \n" << direction << endl;
    }

    Alarm(string d = "abc", string de = "cba", long da = 11, int tr = 6, int di = -1) : Packet(d, de, da)
    {
        threshold = tr;
        direction = di;
    }
};

// ----- LAB_2 ----- //

template <class T, int I>
class Signal
{
public:
    T signal_value[I];
    T signal_fft[I];
    T signal_fft_abs[I];
    float rms;

    Signal()
    {}

    Signal(T signal_value[], T signal_fft[], T signal_fft_abs[], float rms)
    {
        for (int i = 0; i < I; i++)
        {
            this->signal_value[i] = signal_value[i];
            this->signal_fft[i] = signal_fft[i];
            this->signal_fft_abs[i] = signal_fft_abs[i];
        }
        this->rms = rms;
    }

    Signal operator+ (Signal& obj)
    {
        Signal wynik;
        try
        {
            for (int i = 0; i < 3999; i++)
            {
                wynik.signal_fft[i] = signal_fft[i] + obj.signal_fft[i];
                wynik.signal_value[i] = signal_value[i] + obj.signal_value[i];
                wynik.signal_fft_abs[i] = signal_fft_abs[i] + obj.signal_fft_abs[i];
            }

            wynik.rms = rms + obj.rms;
        }
        catch (string w)
        {
            cout << "Error: " << w << endl;
        }
        return wynik;
    }

    Signal operator/ (Signal& obj)
    {
        Signal wynik;
        try
        {
            for (int i = 0; i < 3999; i++)
            {
                wynik.signal_fft[i] = signal_fft[i] / obj.signal_fft[i];
                wynik.signal_value[i] = signal_value[i] / obj.signal_value[i];
                wynik.signal_fft_abs[i] = signal_fft_abs[i] / obj.signal_fft_abs[i];
            }

            wynik.rms = rms / obj.rms;
        }
        catch (string w)
        {
            cout << "Error: " << w << endl;
        }
        return wynik;
    }

    void totxt(Signal s, int nr)
    {
        try
        {
            ofstream myfile;
            myfile.open("signals.txt", ios_base::app);
            myfile << "Signal " << nr << ":\n";
            myfile << "Value:             FFT:             FFT_abs: \n";

            for (int i = 0; i < 3999; i++)
            {
                myfile << s.signal_value[i] << "             " << s.signal_fft[i] << "             " << s.signal_fft_abs[i] << "\n" << endl;
            }

            myfile << "--------------------------------------------\n";
            myfile.close();
        }
        catch (string w)
        {
            cout << "Error: " << w << endl;
        }
    }

};

float gauss(float srednia, float odchylenie)
{
    float fMin, fMax;
    try
    {
        fMin = (srednia - odchylenie);
        fMax = (srednia + odchylenie);
    }
    catch (string w)
    {
        cout << "Error: " << w << endl;
    }
    float f = (float)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


float rms(float wartosci[])
{
    float sum = 0;
    float av = 0;
    float sigmax = 0;
    float x = 0;
    try
    {
        for (int i = 0; i < 3999; i++)
        {
            sum = sum + wartosci[i];
        }

        av = sum / 3999;

        for (int i = 0; i < 3999; i++)
        {
            x += pow((wartosci[i] - av), 2);
        }

        sigmax = sqrt((1 / (float)3998) * x);
    }
    catch (string w)
    {
        cout << "Error: " << w << endl;
    }
    return sigmax;
}

int main()
{
    // ----- LAB_1 ----- //

    TimeHistory p1("ABC1", "ABC1", 99, 1, "Hz", 29.3, 0.92);
    p1.toString();

    Spectrum p2("ABC2", "ABC2", 11, 2, "Hz", 9.5, 1.1);
    p2.toString();

    Alarm p3("ABC3", "ABC2", 44, 6, -1);;
    p3.toString();

    // ----- LAB_2 ----- //

    float s1[4096];
    float f1[4096];
    float s2[4096];
    float f2[4096];
    float s3[4096];
    float f3[4096];
    float s4[4096];
    float f4[4096];
    float fb1[4096];
    float fb2[4096];
    float fb3[4096];
    float fb4[4096];
    float r1;
    float r2;
    float r3;
    float r4;

    float len = 4096;

    ffft::FFTReal <float> fft_object(len);

    for (int i = 0; i < 3999; i++)
    {
        s1[i] = gauss(3, 0.3);
        f1[i] = 0;
        fb1[i] = 0;
        s2[i] = gauss(6, 0.3);
        f2[i] = 0;
        fb2[i] = 0;
        s3[i] = gauss(8, 0.1);
        f3[i] = 0;
        fb3[i] = 0;
        s4[i] = gauss(3, 0.3);
        f4[i] = 0;
        fb4[i] = 0;
    }

    r1 = rms(s1);
    r2 = rms(s2);
    r3 = rms(s3);
    r4 = rms(s4);

    fft_object.do_fft(f1, s1);
    fft_object.do_fft(f2, s2);
    fft_object.do_fft(f3, s3);
    fft_object.do_fft(f4, s4);

    for (int i = 0; i < 3999; i++)
    {
        fb1[i] = abs(f1[i]);
        fb2[i] = abs(f2[i]);
        fb3[i] = abs(f3[i]);
        fb4[i] = abs(f4[i]);
    }

    Signal<float, 3999> sig1(s1, f1, fb1, r1);
    Signal<float, 3999> sig2(s2, f2, fb2, r2);
    Signal<float, 3999> sig3(s3, f3, fb3, r3);
    Signal<float, 3999> sig4(s4, f4, fb4, r4);
    Signal<float, 3999> sig23;
    Signal<float, 3999> sig14;

    swap(sig1, sig2);

    sig23 = sig2 + sig3;
    sig14 = sig1 / sig4;

    remove("signals.txt");

    sig1.totxt(sig1, 1);
    sig2.totxt(sig2, 2);
    sig3.totxt(sig3, 3);
    sig4.totxt(sig4, 4);
    sig23.totxt(sig23, 23);
    sig14.totxt(sig14, 14);

    cout << "RMS1: " << sig1.rms << endl;
    cout << "RMS2: " << sig2.rms << endl;
    cout << "RMS3: " << sig3.rms << endl;
    cout << "RMS4: " << sig4.rms << endl;
    cout << "RMS 2+3 = " << sig23.rms << endl;
    cout << "RMS 1/4 = " << sig14.rms << endl;

    return 0;
}
