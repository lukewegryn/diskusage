#include <QString>
#include <QDebug>
#include <argumentlist.h>
#include <iostream>
#include <QStream.h>
#include <QDirIterator>
#include <QFileInfo>
#include <QDir>

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

void displayHelp()
{
   qout << "\n\t\t\tDISUSAGE" << "\t" << "Luke Wegryn" << endl << endl;
   qout << "./diskusage -depth=(all|<uint>) (-b|-k|-m) -<type> resource0 [resource1 [...]]" << endl << endl;
   qout << "\t-depth=" << "\n\t\t" << "all - display all levels of file information." << "\n\t\t" << "<uint> - specify # of levels to display." << endl << endl;
   qout << "\t-b|-k|-m" << "\n\t\t" << "-b  Sizes are shown in bytes." << "\n\t\t" << "-k  Sizes are shown in kilobytes." << "\n\t\t" << "-m  Sizes are shown in megabytes." << endl << endl;
   qout << "\t-type" << "\n\t\t" << "f  Allow files to be shown." << "\n\t\t" << "d  Allow directories to be shown." << "\n\t\t" << "s Allow symbolic links to be shown." << endl << endl;
   qout << "\tresource0... [resource1 [...]]" << "\n\t\t" << "Specify as many resource file paths as desired with a space in between each one." << endl << endl;
}

void makeDirectoryAlphabetical(QDir dir, QList<QFileInfo> &list)
{
   dir.setSorting(QDir::Name);
   QDir::Filters df = QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks;

   QStringList qsl = dir.entryList(df, QDir::Name | QDir::DirsFirst);
   foreach(const QString &entry, qsl){
      QFileInfo finfo(dir, entry);
      list.append(finfo);
      if(finfo.isDir()){
         QDir sd(finfo.absoluteFilePath());
         makeDirectoryAlphabetical(sd, list);
      }
   }

}
void sizeOutput(QList<QFileInfo> &fileList, QList<bool> bkmfsd, int i)
{
   if(bkmfsd[0])
      qout << fileList[i].size()  << " B" << "\t" << fileList[i].filePath() << endl;
   else if(bkmfsd[1])
      qout << qRound(fileList[i].size()/1024.0) << " kB" << "\t" << fileList[i].filePath() << endl;
   else if(bkmfsd[2])
      qout << qRound(fileList[i].size()/1048576.0) << " MB" << "\t" << fileList[i].filePath() << endl;
}

void displayData(QList<QFileInfo> &fileList, QList<bool> bkmfsd, int i)
{
   QString data = NULL;

   if(bkmfsd[3])
   {
      if(fileList[i].isFile())
      {  
         sizeOutput(fileList,bkmfsd,i);
      }
   }

   if(bkmfsd[4])
   {
      if(fileList[i].isSymLink())
      {  
         sizeOutput(fileList,bkmfsd,i);
      }
   }

   if(bkmfsd[5])
   {
      if(fileList[i].isDir())
      {
         sizeOutput(fileList,bkmfsd,i);
      }
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
      {
         depthString = al.getSwitchArgContains("-depth=", NULL); //depth specification int
         depthString.remove(0,7);
      }

   bytes = al.getSwitch("-b"); //-b|-k|-m
   if(!bytes) roundedkB = al.getSwitch("-k");
   if(!bytes && !roundedkB) roundedMB = al.getSwitch("-m");

   ftype = al.getFlagContains("-f", NULL);
   if(ftype != NULL) setFSD(ftype, f, s, d);
   stype = al.getFlagContains("-s", NULL);
   if(stype != NULL) setFSD(stype, f, s, d);
   dtype = al.getFlagContains("-d", NULL);
   if(dtype != NULL) setFSD(dtype, f, s, d);

   QList<bool> bkmfsd;
   bkmfsd << bytes << roundedkB << roundedMB << f << s << d;

   QString path;
   int initialDepth = 0;
   bool noResources = false;


   QList<QFileInfo> fileList;
   //QStringList pathList;
   if(!al.isEmpty()){
      while(!al.isEmpty()){
         path = al.takeFirst();
         QDir dir(path);
         if(dir.isReadable()){
            makeDirectoryAlphabetical(dir, fileList);
            initialDepth = path.split("/").size();
         }
      }
      //path = "/home/luke/Documents/AppliedSoftwareDesign/diskusage/" + al.takeFirst();
   }

   else
      noResources = true;



   if(!bytes && !roundedkB && !roundedMB) displayHelp();
   else if(!depthAll && depthString == NULL) displayHelp();
   else if(!d && !f && !s) displayHelp();
   else if(noResources) displayHelp();
   else
   {
      for(int i = 0; i < fileList.size(); i++)
      {
         if(depthAll)
         {
            displayData(fileList, bkmfsd, i);
         }

         else
         {
            if((fileList[i].path().split("/").size() - initialDepth) < depthString.toInt())
            {
               displayData(fileList, bkmfsd, i);
            }
         }
      }
   }

}