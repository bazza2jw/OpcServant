//
// (c) B. J. Hill 2008 - 
// Purpose: Correlator user interface
// $Id$
//
#include "Correlator.hpp"

CorrelatorMain::CorrelatorMain(QWidget *p ) : QMainWindow(p)
{
	//
	Data.setupUi(this);
	connect(Data.actionExit,SIGNAL(triggered()), this,SLOT(Exit()));
	connect(Data.actionStart,SIGNAL(triggered()), this,SLOT(Start()));
	connect(Data.actionStop,SIGNAL(triggered()), this,SLOT(Stop()));
	connect(Data.actionCount_Rate,SIGNAL(triggered()), this,SLOT(CountRate()));
	connect(Data.actionClear,SIGNAL(triggered()), this,SLOT(Clear()));
	// connect to the thread signals
	//
	worker = new CorrelatorThread;
	worker->start();
	//
	connect(worker,SIGNAL(CurrentCounts(unsigned)),this,SLOT(CurrentCounts(unsigned)));
	connect(worker,SIGNAL(HaveResults()),this,SLOT(HaveResults()));
	connect(worker,SIGNAL(Fault(unsigned,QString)),this,SLOT(Fault(unsigned,QString)));
	//
	//
	// set the graph pane up
	//
	memset(ChannelCounts,0,sizeof(ChannelCounts));
	memset(ChannelNumber,0,sizeof(ChannelNumber));
	//
	SampleCurve = new QwtPlotCurve("Sample");
	SampleCurve->attach(Data.Plot);
	//
	QwtSymbol sym(QwtSymbol::Diamond, QBrush(Qt::green), QPen(Qt::green), QSize(8,8));
	SampleCurve->setStyle(QwtPlotCurve::Dots);
	SampleCurve->setSymbol(sym);
	//
	Data.Plot->setCanvasBackground (Qt::yellow);
	Data.Plot->setAxisScale ( QwtPlot::xBottom, 0, N_CHANNELS);
	Data.Plot->setAxisTitle ( QwtPlot::yLeft, "Counts");
	Data.Plot->setAxisTitle ( QwtPlot::xBottom, "Channel");
	//
	SemiCurve = new QwtPlotCurve("Semi");
	SemiCurve->attach(Data.SemiLogPlot);
	QwtSymbol SSym(QwtSymbol::Diamond, QBrush(Qt::blue), QPen(Qt::blue), QSize(8,8));
	SemiCurve->setStyle(QwtPlotCurve::Dots);
	SemiCurve->setSymbol(SSym);
	//
	Data.SemiLogPlot->setCanvasBackground (Qt::green);
	Data.SemiLogPlot->setAxisScale ( QwtPlot::xBottom, 0, N_CHANNELS);
	Data.SemiLogPlot->setAxisTitle ( QwtPlot::yLeft, "ln(N)");
	Data.SemiLogPlot->setAxisTitle ( QwtPlot::xBottom, "Channel");
	
	//SamplePhaCurve->setStyle(QwtPlotCurve::Dots);
	SampleCurve->setPen(QPen(Qt::black));
	for(int i = 0; i < N_CHANNELS; i++) ChannelNumber[i] = i;
	//
	QStringList l;
#ifdef UNIX
	QDir dir("/dev");
	QStringList m;
	QDir d("/dev","ttyS*"); // look for ttyS* and ttyACM*
	m << "ttyS*" << "ttyACM*";
	l = dir.entryList(m,QDir::System | QDir::CaseSensitive);
#endif
#ifdef WIN32
	for(int k = 1; k < 16; k++)
{
	l <<  QString("COM%1").arg(k);
};
#endif
	Data.SerialPort->insertItems(0,l);
	Data.SerialPort->setCurrentIndex(0);
};
		
CorrelatorMain::~CorrelatorMain()
{
	if(worker)
	{
		worker->setExit(true);
		delete worker;	
	};
}
		
void CorrelatorMain::Start()
{
	worker->setSampleTime(Data.SampleTime->value());
	worker->setRunTime(Data.RunTime->value());
	worker->setSerialPort(Data.SerialPort->currentText());
	worker->Start();
};
		
void CorrelatorMain::Stop()
{
	worker->Stop();
};
		
void CorrelatorMain::Exit()
{
	
	if(QMessageBox::question (this, "Exit Correlator", "Confirm: Exit Correlator Application",
		 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) == QMessageBox::Ok)
	{
		//
		// termianate threads
		if(worker) worker->setExit(true);
		qApp->quit();
	};
};


void CorrelatorMain::Tick()
{
	
};

void CorrelatorMain::CountRate()
{
	worker->CountRate();
};

void CorrelatorMain::CurrentCounts(unsigned n)
{
	Data.CountRate->display((int)n);
};

void CorrelatorMain::Clear()
{
	worker->Clear();	
};

void CorrelatorMain::HaveResults()
{
	//
	unsigned C[N_CHANNELS];
	memcpy(C,worker->getChannels(),sizeof(C));
	//
	// convert to double
	for (int i = 0; i < N_CHANNELS; i++)
	{
		ChannelCounts[i] = C[i];
	};
	//
	memset(SemiLog,0,sizeof(SemiLog));
	//
	// estimate baseline
	//
	// use the last 10 channels 
	//
	double base = 0;
	int nbase = Data.NumberBaselineChannels->value();
	for(int i = N_CHANNELS - nbase; i < N_CHANNELS; i++)
	{
		base += ChannelCounts[i];
	}; 
	//
	base /= (double)nbase;// determine mean 
	Data.Baseline->setText(QString::number(base));
	//
	// get the counts over
	for (int i = 0; i < (N_CHANNELS - nbase); i++)
	{
		double a = ChannelCounts[i] - base;
		if(a > 0)
		{
			SemiLog[i] = log(a);
		};
	};
	//
	// do a semi log fit
	//
	// do a least squares fit and estimate the gradient
	//
	double sx = 0;
	double sx2 = 0;
	double xy = 0;
	double sy = 0;
	for (int i = 0; i < (N_CHANNELS - nbase); i++)
	{
		sx += ChannelNumber[i];
		sx2 += ChannelNumber[i] * ChannelNumber[i];
		xy += ChannelNumber[i] * SemiLog[i];
		sy += SemiLog[i];
	};
	double an = (N_CHANNELS - nbase);
	double mx = sx / an;
	double my = sy / an;
	double SSxx = sx2 - mx * mx * an;
	double SSxy = xy - an * mx * my;
	//
	if(SSxx > 0)
	{
		double m = SSxy / SSxx;	
		Data.Slope->setText(QString::number(m));
	}
	else
	{
		Data.Slope->setText("Invalid");
	};
	//
	DoPlot();
	// save counts in CONTIN format
};

void CorrelatorMain::Fault(unsigned, QString)
{
	
};

CorrelatorThread::CorrelatorThread(): 
		Exit(false),
		Started(false),
		SerialPort("/dev/ttyS0"),
		SampleTime(4),
		RunTime(10),
		Mode(0),
		LastError(0)
{
	
};
		
CorrelatorThread::~CorrelatorThread() 
{
	
};

//
// set the command
void CorrelatorThread::Clear() // clear / init the correlator
{
	setMode(CLEAR);	
};
void CorrelatorThread::Start() // start correlating
{
	setMode(CORRELATE);
};
void CorrelatorThread::Stop()  // stop correlating
{
	setMode(STOP);
};
void CorrelatorThread::CountRate() // count rate mode
{
	setMode(COUNTRATE);
};
void CorrelatorThread::GetResults() // get the results
{
	setMode(GETRESULTS);
};


// Function:
// Purpose: 
// Inputs: None
// Outputs: None
// Returns: None
//
bool CorrelatorThread::OpenSerial()
{
	bool res = false;
	if(Serial.isOpen()) Serial.Close();
	if(Serial.Open(getSerialPort()))
	{
		res = Serial.Configure(57600) != 0;
	};
	return res;
};

void CorrelatorThread::run()
{
	while(!Exit)
	{
		switch(Mode)
		{
			case IDLE:
			{
				msleep(10);
			};
			break;
			
			case CLEAR:
			{
				// send the clear command
				// open the serial port
				if(OpenSerial())
				{
					Serial.PutChar('C'); 
					msleep(100);
					Serial.Close();
				}
				else
				{
					emit Fault(ERROR,"Cannot Open Serial Port");
				};
			};
			break;
				
			case STOP:
			{
				if(OpenSerial())
				{
					Serial.PutChar('B'); // stop the correlator 
					msleep(100);
					Serial.Close();
				}
				else
				{
					emit Fault(ERROR,"Cannot Open Serial Port");
				};
				Mode=IDLE;
			};
			break;
			
			case COUNTRATE:
			{
				if(OpenSerial())
				{
					Serial.PutChar('A'); // go into count rate mode - until something else is pressed
					while(!Exit && (Mode == COUNTRATE))
					{
						//
						// get lines of text which is the current counts in the last 100ms
						//
						QByteArray b;
						//
						if(Serial.GetLine(b, 100))
						{
							emit CurrentCounts(atoi(b.data()));
						}
						else
						{
							msleep(50);
						};
					};
					Serial.Close();
				}
				else
				{
					emit Fault(ERROR,"Cannot Open Serial Port");
				};
				Mode=IDLE;
			};
			break;
			
			case CORRELATE:
			{
				if(OpenSerial())
				{
					Serial.PutChar('D'); // start correlating
					msleep(100);
					Serial.Close();
				}
				else
				{
					emit Fault(ERROR,"Cannot Open Serial Port");
				};
				Mode=IDLE;
			};
			break;
			
			case GETRESULTS:
			{
				if(OpenSerial())
				{
					Serial.PutChar('E'); // get Results 
					memset(Channels,0,sizeof(Channels));
					//
					int n = 0;
					while(!Exit && (Mode == GETRESULTS))
					{
						QByteArray b;
						//
						if(Serial.GetLine(b, 100))
						{
							Channels[n++] = atoi(b.data());
							if(n >= N_CHANNELS) break;
						}
						else
						{
							msleep(50);
						};
					};
					//
					emit HaveResults(); // say we have downloaded all the results
					Serial.Close();
				}
				else
				{
					emit Fault(ERROR,"Cannot Open Serial Port");
				};
				Mode=IDLE;
			};
			break;
			
			default:
				Mode = IDLE;
				break;
		};
		msleep(50);
	};
};

//
// Entry point
//
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Q_INIT_RESOURCE(Correlator); // get the resources for the typeeditor	
	CorrelatorMain mainWin;
	mainWin.show();
	return app.exec();
}


