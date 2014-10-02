#include <QString>
#include <QDebug>
#include <argumentlist.h>
#include <iostream>
#include <QStream.h>

void processFile(QString filename, bool verbose) {
   if (verbose)
      qDebug() << QString("Do something chatty with %1.")
                         .arg(filename);
   else
      qDebug() << filename;
}

void runTestOnly(QStringList & listOfFiles, bool verbose) {
   foreach (const QString &current, listOfFiles) { 
      processFile(current, verbose);
   }
}

void setFSD(QString argument, bool &f, bool &s, bool &d)
{
   if(argument.contains("f"))
      f = true;
   if(argument.contains("s"))
      s = true;
   if(argument.contains("d"))
      d = true;
}

int main( int argc, char * argv[] ) {
   ArgumentList al(argc, argv);       /* Instantiate the ArgumentList with command line args. */
   QString appname = al.takeFirst();
   bool bytes = false;
   bool roundedkB = false;
   bool roundedMB = false;
   bool depthAll = false;
   QString depthString = NULL;
   bool f = false;
   bool d = false;
   bool s = false;
   QString ftype = NULL;
   QString stype = NULL;
   QString dtype = NULL;

   depthAll = al.getSwitch("-depth=all"); //depth specification all
   if(!depthAll)
      depthString = al.getSwitchArgContains("-depth=", NULL); //depth specification int
   

   bytes = al.getSwitch("-b"); //-b|-k|-m
   if(!bytes)
      roundedkB = al.getSwitch("-k");
   if(!bytes && !roundedkB)
      roundedMB = al.getSwitch("-m");

   ftype = al.getFlagContains("-f", NULL);
   if(ftype != NULL)
      setFSD(ftype, f, s, d);
   
   stype = al.getFlagContains("-s", NULL);
   if(stype != NULL)
      setFSD(stype, f, s, d);

   dtype = al.getFlagContains("-d", NULL);
   if(dtype != NULL)
      setFSD(dtype, f, s, d);


   if(depthAll)
      qout << "depth";
   if(bytes)
      qout << "Bytes";
   if(roundedkB) 
      qout << "kB";
   if(roundedMB)
      qout << "MB";
   if(depthString != NULL)
      qout << depthString;
   if(f)
      qout << "f";
   if(s)
      qout << "s";
   if(d)
      qout << "d";


}

/*QString appname = al.takeFirst();  
   qDebug() << "Running " << appname;
   bool verbose = al.getSwitch("-v");
   bool testing = al.getSwitch("-t"); 
   if (testing) {
      runTestOnly(al, verbose);    
      return 0;
   } else {
      qDebug() << "This Is Not A Test";
   }*/