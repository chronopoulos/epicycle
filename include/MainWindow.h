#include <QWidget>
#include <QGridLayout>
#include <QString>

#include "InportManager.h"
#include "SequenceManager.h"
#include "OutportManager.h"
#include "TransportWidget.h"

#define PLAYING 0
#define PAUSED 1
#define STOPPED 2

class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow();
        bool save(void);
        bool save(const QString&);
        bool saveAs(void);
        void load(void);
        void load(const QString&);
        void clearSession(void);

        void togglePlayState(void);
        QString sessionFile;

    private:
        QHBoxLayout *layout;
        QVBoxLayout *leftLayout;

        InportManager *inportManager;
        SequenceManager *seqManager;
        OutportManager *outportManager;
        TransportWidget *transportWidget;

        int transport;
        bool initialized;

    protected:
        void keyPressEvent(QKeyEvent*);
        void closeEvent(QCloseEvent*);

    public slots:
        void handleDelta(bool);
        void handleTransport(int);
        void handleTempo(double);


};

