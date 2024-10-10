#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QListWidget>
#include <QWidget>
#include"protocol.h"
#include"sharefile.h"

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    //用户目录
    QString m_curPath;//当前所在路径
    QString m_userPath;//用户的根目录
    //移动文件变量
    QString m_mvFileName;
    QString m_mvFilePath;
    //上传文件变量
    QString m_uploadPath;
    bool m_isupload;
    //下载文件变量
    QFile m_downloadFile;
    QString m_downloadFilePath;
    qint64 m_downloadTotalSize;
    qint64 m_hasdownloadSize;
    bool m_isdownload = false;
    //分享文件变量
    ShareFile* m_sharefile;
    QString m_shareFileName;


    QList<FileInfo*> m_pFileInfoList;
    explicit File(QWidget *parent = nullptr);
    void FlushFile();
    void updateFileList(QList<FileInfo*> pFileInfoList);
    void uploadFile();
    void DownloadFile(qint64 fileSize);
    void DownloadFileData(char* buffer,qint64 size);
    ~File();

private slots:
    void on_mKDir_PB_clicked();

    void on_flushFile_PB_clicked();

    void on_delDir_PB_clicked();

    void on_rename_PB_clicked();

    void on_file_LW_itemDoubleClicked(QListWidgetItem *item);


    void on_delFile_PB_clicked();

    void on_upload_PB_clicked();

    void on_return_PB_clicked();

    void on_mvFile_PB_clicked();

    void on_download_PB_clicked();

    void on_shareFile_PB_clicked();

private:
    Ui::File *ui;

};

#endif // FILE_H
