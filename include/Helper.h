#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include "sequoia.h"

QString getRandomString(int);
sq_sequence_t newDefaultSequence(void);
sq_inport_t newDefaultInport(void);
sq_outport_t newDefaultOutport(void);

#endif
