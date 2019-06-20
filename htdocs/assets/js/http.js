//服务器地址
axios.defaults.baseURL = 'http://localhost/xiaosu/';
//响应超时
axios.defaults.timeout = 10000;
//post请求头
axios.defaults.headers.post['Content-Type'] = 'application/json;charset=UTF-8';
//页面样式
var style = document.cookie;
if (style === "")
{
    document.cookie = "layout=boxed";
    document.cookie = "bg=bg2";
}
// 响应拦截器
axios.interceptors.response.use(
    response =>
    {
        //如果返回的状态码为200
        if (response.status === 200) 
        {
            return Promise.resolve(response);        
        }
        else//否则的话抛出错误
        {
            return Promise.reject(response);        
        }
    },
    //后台错误状态码
    error => 
    {           
        if (error.response.status) 
        {
            switch (error.response.status) 
            {
                case 404:
                    break;
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
    return new Promise((resolve, reject) =>
    {        
        axios.get(url + (params == undefined ? "" : params))
        .then(res => 
        {
            resolve(res.data);
        })
        .catch(err =>
        {
            reject(err.data)
        })
    });
}

/** 
 * post方法，对应post请求 
 * @param {String} url [请求的url地址] 
 * @param {Object} params [请求时携带的参数] 
 */
function post(url, params) 
{
    return new Promise((resolve, reject) => 
    {
        axios.post(url, params)
        .then(res => 
        {
            resolve(res.data);
        })
        .catch(err =>
        {
            reject(err.data)
        })
    });
}

function timetrans(date)
{
    var date = new Date(date);
    var Y = date.getFullYear() + '/';
    var M = (date.getMonth()+1 < 10 ? '0'+(date.getMonth()+1) : date.getMonth()+1) + '/';
    var D = (date.getDate() < 10 ? '0' + (date.getDate()) : date.getDate()) + ' ';
    var h = (date.getHours() < 10 ? '0' + date.getHours() : date.getHours()) + ':';
    var m = (date.getMinutes() <10 ? '0' + date.getMinutes() : date.getMinutes()) + ':';
    var s = (date.getSeconds() <10 ? '0' + date.getSeconds() : date.getSeconds());
    return Y + M + D + h + m + s;
}

function spopAlert(strMessage, strStyle, strPosition) 
{
    spop({
        template: strMessage,
        position: strPosition,          //top-left, top-center, bottom-left, bottom-center, bottom-right
        style: strStyle,                //info, error, success
        autoclose: 5000,                //false, miliseconds
        icon: true,                     //true or false
        group: false                    //string, add a id reference 
    });
}
