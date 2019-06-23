var nId = 0;
let url = new URL(window.location.href);

window.onload = function() 
{
    var json = JSON.parse(localStorage.getItem("list"));

    if (!url.searchParams.has("single"))
    {
        return;
    }
    nId = url.searchParams.get("single");
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
        if (article.data.image != "") 
        {
            $("#header").append("<div class='media'><img src='"+ article.data.image +"' alt=''></div>");
        }
        $("#header").append("<h3><a>"+article.data.title+"</a></h3><span>"+timetrans(article.data.time)+" / by "+article.data.user.displayname+" / in: "+article.data.sort.name+" / "+ article.data.comment_count +" Comments</a></span>");
        $("#content").append(article.data.content);
        $("#comment").html(article.data.comment_count + " Comments");

        $("#post-author").append("<h4><a href='category.html?author=\""+article.data.user.id+"\"'>"+article.data.user.displayname+"</a></h4>");
        $("#post-author").append("<p>"+ article.data.user.signature +"</p>");
        if (article.data.user.profile_photo != "") 
        {
            $("#post-author").append("<img src='"+article.data.user.profile_photo+"' alt=''>");
        }
        else
        {
            $("#post-author").append("<img src='assets/img/avatar.png' alt=''>");
        }
        document.title = article.data.title + " - 小蘇-XiaoSU";
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
        if (comment.data === "null") 
        {
            return;
        }

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
                                "+timetrans(obj.time)+" /&nbsp;&nbsp;<a href='javascript:void(0)' onclick='reply(\""+obj.user.displayname+"\","+obj.id+")' class='comment-edit-link'>回复</a> \
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

var parentId = 0;
function reply(userDisplayName, commentId) 
{
    parentId = commentId;
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
            $("#post-nav-2").append("<div class='push-left'><a href='single.html?single="+json[i + 1].id+")'>« Previous</a><h6> "+json[i-1].title+" </h6></div>");
        }
        else
        {
            $("#post-nav-2").append("<div class='push-left'>« Previous<h6>没有了</h6></div>");
        }

        if (json[i + 1] != undefined) 
        {
            $("#post-nav-2").append("<div class='push-right'><a href='single.html?single="+json[i + 1].id+"'>Next »</a><h6>"+json[i+1].title+"</h6></div>");
        }
        else
        {
            $("#post-nav-2").append("<div class='push-right'>Next »<h6>没有了</h6></div>");
        }
    }
}

function sendComment() 
{

    spopAlert("评论系统正在施工!", "error", "bottom-right");
    return;

    let data = new FormData();
    let json = "{\"id\":0, \"user\":1, \"article\":"+nId+" ,\"like_count\":"+0+" ,\"time\": "+Math.round(new Date() / 1000)+",\"content\": \""+$("#comment_text").val()+"\",\"parent\":"+parentId+"}";
    data.append('comment',json);

    post_comment(data)
    .then(function(response) {
        console.log(response);
        if (response.error === "null") 
        {
            comment_byid("/"+response.inseredid)
            .then(function(response) {
                var strInfo = "";
                var image = "";
                strInfo = response.data.user.displayname;
                if (response.data.user.profile_photo === "")
                {
                    image = "<img src='assets/img/avatar.png' alt=''>";
                }
                else
                {
                    image = "<img src='"+ response.data.user.profile_photo +"' alt=''>";
                }
                var html = "<li class='comment byuser even thread-even depth-1'> \
                        <div class='comment-body'> \
                            <div class='comment-author vcard'> \
                                "+image+" \
                                <cite class='fn'>"+strInfo+"</cite> \
                            </div> \
                            <p>"+response.data.content+"</p> \
                            <div class='comment-meta commentmetadata'> \
                                "+timetrans(response.data.time)+" /&nbsp;&nbsp;<a href='javascript:void(0)' onclick='reply(\""+response.data.user.displayname+"\","+response.data.id+")' class='comment-edit-link'>回复</a> \
                            </div> \
                        </div> \
                    </li>";
                $("#list-comments").append(html);
            })
            .catch (function(error) {
                spopAlert(error.error, "error", "bottom-right");
            });
            $("#comment_count").text(count);
            spopAlert(response.data, "success", "bottom-right");
        }
        else
        {
            spopAlert(response.error, "error", "bottom-right");
        }
    })
    .catch(function(error) {
        console.log(error);
    });
}
