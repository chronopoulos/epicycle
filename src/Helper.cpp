#include <QDebug>
#include <QRandomGenerator>

#include "Helper.h"

extern sq_session_t SESSION;

QString getRandomString(int length) {


   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

   QRandomGenerator *rand = QRandomGenerator::global();

   QString randomString;
   for(int i=0; i<length; ++i)
   {
       int index = rand->generate() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

sq_sequence_t newDefaultSequence(void) {

    sq_sequence_t seq;

    seq = sq_sequence_new(16);
    sq_sequence_set_name(seq, getRandomString(4).toStdString().c_str());
    sq_sequence_set_notifications(seq, true);
    sq_session_add_sequence(SESSION, seq);

    return seq;

}

sq_inport_t newDefaultInport(void) {

    sq_inport_t inport;

    inport = sq_inport_new(getRandomString(4).toStdString().c_str());
    sq_session_register_inport(SESSION, inport);

    return inport;

}

sq_outport_t newDefaultOutport(void) {

    sq_outport_t outport;

    outport = sq_outport_new(getRandomString(4).toStdString().c_str());
    sq_session_register_outport(SESSION, outport);

    return outport;

}

