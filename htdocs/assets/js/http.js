//服务器地址
axios.defaults.baseURL = 'http://localhost/xiaosu/';
//响应超时
axios.defaults.timeout = 10000;
//post请求头
axios.defaults.headers.post['Content-Type'] = 'application/x-www-form-urlencoded;charset=UTF-8';
// 响应拦截器
axios.interceptors.response.use(    
    response => {
        // 如果返回的状态码为200
        if (response.status === 200) 
        {
            return Promise.resolve(response);        
        }
        else// 否则的话抛出错误
        {
            return Promise.reject(response);        
        }
    },    
    // 后台错误状态码
    error => {           
        if (error.response.status) 
        {
            switch (error.response.status) 
            {
                // 401: 未登录          
                case 401:
                    break;
                // 403 token过期
                case 403:
                    break; 
                // 404请求不存在
                case 404:
                    break;
                // 其他错误，直接抛出错误提示
                default:
                    break;
            }
            return Promise.reject(error.response);
        }
    }
);

/**
 * get方法，对应get请求
 * @param {String} url [请求的url地址]
 * @param {Object} params [请求时携带的参数]
 */
function get(url, params)
{
    return new Promise((resolve, reject) =>{        
        axios.get(url + (params==undefined?"":params)).then(res => {
            resolve(res.data);
        }).catch(err =>{
            reject(err.data)        
    })    
});}

/** 
 * post方法，对应post请求 
 * @param {String} url [请求的url地址] 
 * @param {Object} params [请求时携带的参数] 
 */
function post(url, params) 
{
    return new Promise((resolve, reject) => {
         axios.post(url, QS.stringify(params))
        .then(res => {
            resolve(res.data);
        })
        .catch(err =>{
            reject(err.data)
        })
    });
}

//链接类型枚举
var LinkTypeEnum = { TEXT: 1, AUTHOR: 2, TRAVEL: 3, COMMENTS: 4 };

function pageJump(type, uuid)
{
    localStorage.setItem("type", type);
    localStorage.setItem("uuid", uuid);

    switch (type)
    {
    case LinkTypeEnum.TEXT:
        self.location = 'single.html';
        break;
    case LinkTypeEnum.AUTHOR:
        self.location = 'category.html';
        break;
    case LinkTypeEnum.TRAVEL:
        self.location = 'category.html';
        break;
    case LinkTypeEnum.COMMENTS:
        self.location = 'single.html#comments';
        break;
    default:
        self.location = '/blog/';
        break;
    }
}

/*
1.localStorage - 没有时间限制的数据存储 
　var arr=[1,2,3];
　localStorage.setItem("temp",arr); //存入 参数： 1.调用的值 2.所要存入的数据 
　console.log(localStorage.getItem("temp"));//输出

2.清空localStorage
　localStorage.clear(); //

3.删除键值对
　localStorage.removeItem("arr");　　
　注意：存入的数据只能以字符串形式存入。

三.提供转JOSN数据方法：
　//JSON对象转JSON字符串
　var obj = {"a": 1,"b": 2};
　obj = JSON.stringify(obj); //转化为JSON字符串
　localStorage.setItem("temp2", obj);
　//JSON字符串转JSON对象
　obj=JSON.parse(localStorage.getItem("temp2"));
*/
