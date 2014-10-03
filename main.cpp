#include <QString>
#include <QDebug>
#include <argumentlist.h>
#include <iostream>
#include <QStream.h>
#include <QDirIterator>
#include <QFileInfo>

void setFSD(QString argument, bool &f, bool &s, bool &d)
{
   if(argument.contains("f"))
      f = true;
   if(argument.contains("s"))
      s = true;
   if(argument.contains("d"))
      d = true;
}

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

void makeDirectory(QDirIterator &itr, QList<QFileInfo> &list)
{
   static int currentDepth = 0;
   if(itr.hasNext())
   {
      itr.next();
      list.append(itr.fileInfo());
      //qDebug() << itr.fileName();
      makeDirectory(itr, list);
   }
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
   //al.takeFirst();
   //runTestOnly(al, false);

   QList<QFileInfo> fileList;

   if(!al.isEmpty())
   {
      QDirIterator itr("/home/luke/Documents/AppliedSoftwareDesign/diskusage/" + al.takeFirst(),QDirIterator::Subdirectories); //| QDirIterator::FollowSymlinks); //
      makeDirectory(itr, fileList);
   }

   else
      qout << "You forgot to specify a resource!";

   for(int i = 0; i < fileList.size(); i++)
   {
      //qout << fileList[i].fileName() << endl;
      /*if(fileList[i].isDir())
      {
         qout << fileList[i].size() << " " << fileList[i].fileName() << endl;
      }

      else if(fileList[i].isFile())
      {
         qout << "\t" << fileList[i].size() << "\t" << fileList[i].fileName() << endl;
      }*/
      qout << fileList[i].size() << "\t" << fileList[i].filePath() << "\n";
   }
}