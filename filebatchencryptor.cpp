#include "filebatchencryptor.h"
#include "util.h"

FileBatchEncryptor::FileBatchEncryptor(
    QByteArray key,
    FileExistAction fileExistAction,
    bool deleteProcessedFiles)
{
    this->key = key;
    this->fileExistAction = fileExistAction;
    this->deleteProcessedFiles = deleteProcessedFiles;
}

void FileBatchEncryptor::processDirectory(QDir &inDir, QDir &outDir, QStringList masks)
{
    if (!inDir.exists()) inDir.mkpath(".");
    if (!outDir.exists()) outDir.mkpath(".");

    inDir.setNameFilters(masks);
    QStringList files = inDir.entryList(QDir::Files);
    for (const QString &filename : std::as_const(files))
    {
        QFile inFile(inDir.filePath(filename));
        QFile outFile(outDir.filePath(filename));
        processFile(inFile, outFile);
    }
}

void FileBatchEncryptor::processFile(QFile &inFile, QFile &outFile)
{
    if (isFileLocked(inFile.fileName())) return;

    if (fileExistAction == FileExistAction::AppendCounter)
    {
        QString filename = outFile.fileName();
        for (int i = 1;; ++i)
        {
            outFile.setFileName(filename + "." + QString::number(i));
            if (!outFile.exists()) break;
        }
    }

    if (inFile.open(QIODevice::ReadOnly) && outFile.open(QIODevice::WriteOnly)) {
        char buf[1024];
        int keyIdx = 0;
        quint64 bytesRead;
        while (0 < (bytesRead = inFile.read(buf, 1024)))
        {
            for (int i = 0; i < bytesRead; ++i) {
                buf[i] ^= key[keyIdx++];
                if (keyIdx >= key.size()) keyIdx = 0;
            }
            outFile.write(buf, bytesRead);
        }

        inFile.close();
        outFile.close();

        if (deleteProcessedFiles)
            inFile.remove();
    }
}
