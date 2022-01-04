#include <Ougens.h>
#include <vector>
		  // the base class for this instrument
typedef struct {float waveSampInc; float ampSampInc; float wavePhase; float ampPhase; int dur; float panR; float panL; int currTime; bool isplaying;} Grain;

class AUDIOBUFFER {
public:
        AUDIOBUFFER(int size);
        ~AUDIOBUFFER();
        double Get(int index);
        void Append(double samp);
        int LimitSize();  // returns the number of grains we have to shift back
        void Print();

private:
        std::vector<double>* _buffer;
        int _maxSize;
};

class STGRAN2 : public Instrument {

public:
	STGRAN2();
	virtual ~STGRAN2();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();
	void addgrain();
	double prob(double low,double mid,double high,double tight);
	void resetgrain(Grain* grain);
	void resetgraincounter();
	int calcgrainsrequired();

private:
	int branch;

	double transLow;
	double transMid;
	double transHigh;
	double transTight;

	double grainDurLow;
	double grainDurMid;
	double grainDurHigh;
	double grainDurTight;

	double panLow;
	double panMid;
	double panHigh;
	double panTight;

	float amp;

	std::vector<Grain*>* grains;
	int newGrainCounter;

	int grainRateSamps;
	double grainRateVarLow;
	double grainRateVarMid;
	double grainRateVarHigh;
	double grainRateVarTight;

	int grainsRequired;
	double* wavetable;
	int wavetableLen;
	double* grainEnv;
	int grainEnvLen;
	float grainRate;
	void doupdate();
};

