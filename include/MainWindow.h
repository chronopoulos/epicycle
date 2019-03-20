#include <QWidget>
#include <QGridLayout>
#include <QString>

#include "InportManager.h"
#include "SequenceManager.h"
#include "OutportManager.h"

#define PLAYING 0
#define PAUSED 1
#define STOPPED 2

class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow();

        void togglePlayState(void);

    private:
        QHBoxLayout *layout;

        InportManager *inportManager;
        SequenceManager *seqManager;
        OutportManager *outportManager;

        int transport;

    protected:
        void keyPressEvent(QKeyEvent*);
        void closeEvent(QCloseEvent*);


};
