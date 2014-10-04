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

void makeDirectory(QDirIterator &itr, QList<QFileInfo> &list)
{
   if(itr.hasNext())
   {
      itr.next();
      list.append(itr.fileInfo());
      makeDirectory(itr, list);
   }
}

void makeDirectoryAlphabetical(QDirIterator &itr, QList<QFileInfo>&list)
{
   if(itr.hasNext())
   {
      itr.next();
      list.append(itr.fileInfo());
      makeDirectory(itr, list);
   }
}

void displayData(QList<QFileInfo> &fileList, bool b, bool k, bool m, int i)
{
   if(b)
         qout << fileList[i].size()  << " B" << "\t" << fileList[i].fileName() << "\n";
   else if(k)
         qout << qRound(fileList[i].size()/1024.0) << " kB" << "\t" << fileList[i].fileName() << "\n";
   else if(m)
         qout << qRound(fileList[i].size()/1048576.0) << " MB" << "\t" << fileList[i].fileName() << "\n";
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
   QString path;
   int initialDepth = 0;

   /////////Trial//////////
   
   /////////End Trial//////

   if(!al.isEmpty())
   {
      path = "/home/luke/Documents/AppliedSoftwareDesign/diskusage/" + al.takeFirst();
      initialDepth = path.split("/").size() + 1;
      QDirIterator itr(path,QDirIterator::Subdirectories | QDirIterator::FollowSymlinks); //
      makeDirectory(itr, fileList);
   }

   else
      qout << "You forgot to specify a resource!" << endl;

   for(int i = 0; i < fileList.size(); i++)
   {
      if(depthAll)
      {
         displayData(fileList, bytes, roundedkB, roundedMB, i);
      }

      else
      {
         if((fileList[i].path().split("/").size() - initialDepth) < depthString.toInt())
         {
            displayData(fileList, bytes, roundedkB, roundedMB, i);
         }
      }
   }

}