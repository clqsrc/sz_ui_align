



//2023 windows 下路径 d:\ 会产生奇怪的文件名，所以要处理一下
func MakeFileName(path, fn string) string {

    // switch runtime.GOOS {
    // case "windows":
    //     cmd = "cmd"
    //     args = []string{"/c", "start", filename}
    // case "darwin":
    //     cmd = "open"
    //     args = []string{filename}
    // case "linux":
    //     cmd = "xdg-open"
    //     args = []string{filename}
    // default:
    //     return fmt.Errorf("unsupported platform")
    // }

	//----------------

	r := path;

	//在 windows 下，如果最后一个字符是 "\" 或者 "/" 是要去掉的
	if len(r) > 1 { 

		
		switch runtime.GOOS {
		case "windows":
			// cmd = "cmd"
			// args = []string{"/c", "start", filename}

			last_c := r[len(r)-1];

			if (last_c == '\\')||(last_c == '/') {
				r = r[:len(r)-1];
			}

		case "darwin":
			// cmd = "open"
			// args = []string{filename}
		case "linux":
			// cmd = "xdg-open"
			// args = []string{filename}
		default:
			//return fmt.Errorf("unsupported platform")
		}//switch

	}//if 

	r = r + "/" + fn;

	//win10 的 mklink 中的路径分隔一定要是 "\" 不能是 "/"
	if ("windows" == runtime.GOOS) {
		r = str_replace(r, "/", "\\");

	}//

	return r;
}//

//只改路径分隔符的版本
func MakeFileName_forPathSP(fn string) string {

    // switch runtime.GOOS {
    // case "windows":
    //     cmd = "cmd"
    //     args = []string{"/c", "start", filename}
    // case "darwin":
    //     cmd = "open"
    //     args = []string{filename}
    // case "linux":
    //     cmd = "xdg-open"
    //     args = []string{filename}
    // default:
    //     return fmt.Errorf("unsupported platform")
    // }

	//----------------

	r := fn;


	//r = r + "/" + fn;

	//win10 的 mklink 中的路径分隔一定要是 "\" 不能是 "/"
	if ("windows" == runtime.GOOS) {
		r = str_replace(r, "/", "\\");

	}//

	return r;
}//


