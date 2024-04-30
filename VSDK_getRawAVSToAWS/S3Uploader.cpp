//#include "S3Uploader.h"
//#include <aws/core/Aws.h>
//#include <aws/s3/S3Client.h>
//#include <aws/s3/model/PutObjectRequest.h>
//#include <fstream>
//
//S3Uploader::S3Uploader(const Aws::String& bucketName, const Aws::String& region) : m_bucketName(bucketName) {
//    Aws::Client::ClientConfiguration clientConfig;
//    clientConfig.region = region;
//    m_s3Client = Aws::S3::S3Client(clientConfig);
//}
//
//bool S3Uploader::UploadFile(const std::string& filePath, const std::string& objectName) {
//    Aws::S3::Model::PutObjectRequest request;
//    request.WithBucket(m_bucketName).WithKey(objectName.c_str());
//
//    std::shared_ptr<Aws::IOStream> inputDataStream = Aws::MakeShared<Aws::FStream>("UploadTag", filePath.c_str(), std::ios_base::in | std::ios_base::binary);
//    request.SetBody(inputDataStream);
//
//    auto outcome = m_s3Client.PutObject(request);
//
//    if (!outcome.IsSuccess()) {
//        // Error handling here
//        return false;
//    }
//    return true;
//}
