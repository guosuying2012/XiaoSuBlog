
window.onload = function() 
{
    var nId = localStorage.getItem("article_id");
    var json = JSON.parse(localStorage.getItem("list"));
    getArticle(nId);
    setPreviousNextJson(json, nId);
}

function getArticle(articleId) 
{
    article("/" + articleId)
    .then(article=> 
    {
        getUser(article.data.user.id);
        getComment(articleId);

        $("#header").append("<h3><a>"+article.data.title+"</a></h3><span>"+timetrans(article.data.time)+" / "+ article.data.comment_count +" Comments</a></span>");
        if (article.data.image != "") 
        {
            $("#header").append("<div class='media'><img src='"+ article.data.image +"' alt=''></div>");
        }
        $("#content").append(article.data.content);
        $("#comment").html(article.data.comment_count + " Comments");
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}

function getUser(userId) 
{
    user("/" + userId)
    .then(user=> 
    {
        $("#post-author").append("<h4><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+user.data.id+"\")'>"+user.data.nikename+"</a></h4>");
        $("#post-author").append("<p>"+ user.data.signature +"</p>");
        if (user.data.profile_photo != "") 
        {
            $("#post-author").append("<img src='"+user.data.profile_photo+"' alt=''>");
        }
        else
        {
            $("#post-author").append("<img src='assets/img/avatar.png' alt=''>");
        }
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}

function getComment(articleId) 
{
    comment("/" + articleId)
    .then(comment=> 
    {
        var html = "<li class='comment byuser even thread-even depth-1'> \
                        <div class='comment-body'> \
                            <div class='comment-author vcard'> \
                                <img src='assets/img/avatar.png' alt=''> \
                                <cite class='fn'>name&nbsp;/&nbsp;回复(name)</cite> \
                            </div> \
                            <p></p> \
                            <div class='comment-meta commentmetadata'> \
                                time /&nbsp;&nbsp;<a href='javascript:void(0)' onclick='sendComment("+1+")' class='comment-edit-link'>回复</a> \
                            </div> \
                        </div> \
                    </li>";
        $("#list-comments").html(html);
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}

function sendComment(commentId) 
{
    console.log(commentId);
}

function setPreviousNextJson(json, nId) 
{
    for (var i = 0; i <= json.length - 1; ++i) 
    {
        if (nId != json[i].id) 
        {
            continue;
        }

        if (json[i - 1] != undefined) 
        {
            console.log("上一篇" + json[i - 1].title);
            $("#post-nav-2").append("<div class='push-left'><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+json[i - 1].id+"\")'>« Previous</a><h6> "+json[i-1].title+" </h6></div>");
        }
        /*else
        {
            $("#post-nav-2").append("<div class='push-left'>« Previous<h6>没有了</h6></div>");
        }*/

        if (json[i + 1] != undefined) 
        {
            console.log("下一篇" + json[i + 1].title);
            $("#post-nav-2").append("<div class='push-right'><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+json[i + 1].id+"\")'>Next »</a><h6>"+json[i+1].title+"</h6></div>");
        }
        /*else
        {
            $("#post-nav-2").append("<div class='push-right'>Next »<h6>没有了</h6></div>");
        }*/
    }
}


