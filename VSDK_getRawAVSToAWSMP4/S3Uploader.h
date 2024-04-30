//#ifndef S3UPLOADER_H
//#define S3UPLOADER_H
//
//#include <aws/core/Aws.h>
//#include <aws/s3/S3Client.h>
//#include <aws/s3/model/PutObjectRequest.h>
//#include <string>
//
//class S3Uploader {
//public:
//    S3Uploader(const Aws::String& bucketName, const Aws::String& region = "us-east-1");
//    bool UploadFile(const std::string& filePath, const std::string& objectName);
//
//private:
//    Aws::String m_bucketName;
//    Aws::S3::S3Client m_s3Client;
//};
//
//#endif // S3UPLOADER_H
