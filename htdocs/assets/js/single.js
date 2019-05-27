
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
        //getUser(article.data.user.id);
        getComment(articleId);

        $("#header").append("<h3><a>"+article.data.title+"</a></h3><span>"+timetrans(article.data.time)+" / by "+article.data.user.displayname+" / in: "+article.data.sort.name+" / "+ article.data.comment_count +" Comments</a></span>");
        if (article.data.image != "") 
        {
            $("#header").append("<div class='media'><img src='"+ article.data.image +"' alt=''></div>");
        }
        $("#content").append(article.data.content);
        $("#comment").html(article.data.comment_count + " Comments");

        $("#post-author").append("<h4><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.AUTHOR, \""+article.data.user.id+"\")'>"+article.data.user.displayname+"</a></h4>");
        $("#post-author").append("<p>"+ article.data.user.signature +"</p>");
        if (article.data.user.profile_photo != "") 
        {
            $("#post-author").append("<img src='"+article.data.user.profile_photo+"' alt=''>");
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

/*function getUser(userId) 
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
}*/

function getComment(articleId) 
{
    comment("/" + articleId)
    .then(comment=> 
    {
        for (var i = 0; i <= comment.data.length - 1; ++i) 
        {
            var obj = comment.data[i];
            var strInfo = "";
            var image = "";
            if (obj.parent == 0) 
            {
                strInfo = obj.user.displayname;
            }
            else
            {
                comment.data.forEach(function (v,i) 
                {
                    if (v.id === obj.parent) 
                    {
                        strInfo = obj.user.displayname + '&nbsp;(Reply:' + v.user.displayname + ')';
                        return;
                    }
                });
            }
            if (obj.user.profile_photo === "")
            {
                image = "<img src='assets/img/avatar.png' alt=''>";
            }
            else
            {
                image = "<img src='"+ obj.user.profile_photo +"' alt=''>";
            }
            var html = "<li class='comment byuser even thread-even depth-1'> \
                        <div class='comment-body'> \
                            <div class='comment-author vcard'> \
                                "+image+" \
                                <cite class='fn'>"+strInfo+"</cite> \
                            </div> \
                            <p>"+obj.content+"</p> \
                            <div class='comment-meta commentmetadata'> \
                                "+timetrans(obj.time)+" /&nbsp;&nbsp;<a href='javascript:void(0)' onclick='sendComment(\""+obj.user.displayname+"\","+obj.id+")' class='comment-edit-link'>回复</a> \
                            </div> \
                        </div> \
                    </li>";
            $("#list-comments").append(html);
        }
        
    })
    .catch(err=>
    {
        spopAlert(err.error, "error", "bottom-right");
        return;
    });
}

function sendComment(userDisplayName, commentId) 
{
    console.log(userDisplayName + commentId);
    self.location = '#commentform';
    document.querySelector('textarea').setAttribute('placeholder', "回复" + userDisplayName);
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
            $("#post-nav-2").append("<div class='push-left'><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+json[i - 1].id+"\")'>« Previous</a><h6> "+json[i-1].title+" </h6></div>");
        }
        /*else
        {
            $("#post-nav-2").append("<div class='push-left'>« Previous<h6>没有了</h6></div>");
        }*/

        if (json[i + 1] != undefined) 
        {
            $("#post-nav-2").append("<div class='push-right'><a href='javascript:void(0)' onclick='pageJump(LinkTypeEnum.TEXT, \""+json[i + 1].id+"\")'>Next »</a><h6>"+json[i+1].title+"</h6></div>");
        }
        /*else
        {
            $("#post-nav-2").append("<div class='push-right'>Next »<h6>没有了</h6></div>");
        }*/
    }
}


