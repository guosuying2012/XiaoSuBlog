window.onload = function() 
{
    sendRequest();
};

//文章列表
function article_callback(response) 
{
    var articles = [];
    for (var i = 0; i <= response.data.length - 1; ++i) 
    {
        var obj = response.data[i];
        var article = "";
        article = "<article class='post'> <header>";
        if (obj.image != "")
        {
            article += "<div class='media'><a href='single.html?single="+obj.id+"'><img src='"+obj.image+"' alt='"+obj.title+"' /></a></div>";
        }
        article += "<h3><a href='single.html?single="+obj.id+"'>"+obj.title+"</a></h3> \
        <span><span>"+timetrans(obj.time)+"</span> \
        / by <a href='category.html?author="+obj.user.id+"'><span>"+ obj.user.displayname +"</span></a> \
        / in: <span><a href='category.html?category="+obj.sort.id+"'>"+obj.sort.name+"</a></span> \
        </header> <div class='editor-styles' style='text-indent:2em;word-wrap:break-word;'>"+obj.describe+"......</div> <footer> <div> \
        <a href='single.html?single="+obj.id+"''>Continue Reading...</a> \
        </div><hr> </footer> </article>";
        $("#list").append(article);

        var data = {};
        data["id"] = obj.id;
        data["title"] = obj.title;
        articles.push(data);
    }
    localStorage.setItem("list", JSON.stringify(articles));
}

var nNumber = 1;
function loadmore() 
{
    ++nNumber;
    sendRequest();
}

function sendRequest()
{
    axios.all([article_list("/" + nNumber)])
    .then(axios.spread(function (resArticles) 
    {
        if (resArticles.data != "null") 
        {
            article_callback(resArticles);
        }
        else
        {
            spopAlert(resArticles.error, "error", "bottom-right");
            return;
        }

        var post = document.getElementById("post-nav");
        if (post === null) 
        {
            $("#content").append("<nav id='post-nav'><a href='javascript:void(0)' onclick='loadmore()'>Older Posts »</a></nav>");
        }
    }))
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}
