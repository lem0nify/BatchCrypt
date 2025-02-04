#ifndef FILEBATCHENCRYPTOR_H
#define FILEBATCHENCRYPTOR_H

#include <QDir>

enum FileExistAction {
    Overwrite = 0,
    AppendCounter = 1,
};

class FileBatchEncryptor
{
public:
    FileBatchEncryptor(
        QByteArray key,
        FileExistAction fileExistAction = FileExistAction::Overwrite,
        bool deleteProcessedFiles = false);

    void processDirectory(QDir &inDir, QDir &outDir, QStringList masks);
    void processFile(QFile &inFile, QFile &outFile);

public:
    FileExistAction fileExistAction;
    bool deleteProcessedFiles;

private:
    QByteArray key;
};

#endif // FILEBATCHENCRYPTOR_H
