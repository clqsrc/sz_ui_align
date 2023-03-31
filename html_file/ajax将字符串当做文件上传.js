
// [js/html] ajax 将字符串当做文件上传

// 示例函数中的参数都是字符串

//上传文件
function StringUploadAsFile(fileName, content, sessionId, userName) {

    const file = new Blob([content], { type: 'multipart/form-data' });

    // 使用 FormData 对象添加文件
    const formData = new FormData();
    formData.append('uploadedFile', file, fileName + ".dat");

    formData.append("id", sessionId);
    formData.append("user_name", userName);

    $.ajax({
        type: 'post',
        url: "http://127.0.0.0:8080/file_upload.jsp",
        data: formData,
        cache: false,
        processData: false,
        contentType: false,
    }).success(function (data) {
        if (data.result == 1) {
            alert("上传成功");
        } else {
            alert("上传失败:" + data.error);
        }
        console.log(data, "---res---")
    }).error(function () {
        alert("上传失败");
    });
}//