# include "QStream.h"

QTextStream qout(stdout, QIODevice::WriteOnly);
QTextStream qerr(stderr, QIODevice::WriteOnly);
QTextStream qin (stdin , QIODevice::ReadOnly );
