#include <Ougens.h>		  // the base class for this instrument

class SIMPLEOSC : public Instrument {

public:
	SIMPLEOSC();
	virtual ~SIMPLEOSC();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();

private:
	void doupdate();
	Ooscili *theOscil;
};

