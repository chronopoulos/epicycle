#include <QWidget>
#include <QGridLayout>
#include <QString>

#include "Manager.h"
#include "Editor.h"
#include "NotificationThread.h"

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
        QGridLayout *layout;

        Manager *manager;
        Editor *editor;

        int transport;

        NotificationThread *notiThread;

    protected:
        void keyPressEvent(QKeyEvent*);
        void closeEvent(QCloseEvent*);


};
