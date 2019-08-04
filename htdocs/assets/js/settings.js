//滚动图片
function slider_callback(response) 
{
    for (var i = response.data.length - 1; i >= 0; i--) 
    {
        var slider = $("<div><a><img class='slider_image' style='width:265px;height:200px'></a><span class='description'></span></div>");
        $("#featured").append(slider);
    }

    var slider_imgs = document.getElementsByClassName("slider_image");
    var description = document.getElementsByClassName("description");
    for (var i = slider_imgs.length - 1; i >= 0; i--) 
    {
        slider_imgs[i].setAttribute("src", response.data[i]["path"]);
        slider_imgs[i].setAttribute("alt", response.data[i]["description"]);
        description[i].innerHTML = response.data[i]["description"];
    }
}

//导航栏
function navigation_callback(response) 
{
    //思路参考：https://blog.csdn.net/Mr_JavaScript/article/details/82817177
    let cloneData = JSON.parse(JSON.stringify(response.data));
    var tree = cloneData.filter(father=>{
        let branchArr = cloneData.filter(child=>father.id == child.parentId);
        branchArr.length>0 ? father.children=branchArr : '';
        return father.parentId==0;
    });
    tree.unshift({'id':10086, 'name':'Home', 'parentId':0, 'rank':1});
    tree.push({'id':10010, 'name':'About', 'parentId':0, 'rank':1});
    navigation(tree, "#menu");
}

function website_callback(response) 
{
    for (var i = response.data.length - 1; i >= 0; i--) 
    {
        document.getElementsByClassName(response.data[i].name).innerHTML = response.data[i].value;
    }
}

//思路参考：https://blog.csdn.net/cc_fys/article/details/81284638
function navigation(tree, parentElement) 
{
    for (var i = 0; i <= tree.length - 1; ++i)
    {
        var obj = tree[i];
        var element = document.createElement('li');
        if (obj.children != undefined)
        {
            element.innerHTML = "<a href='category.html?category="+obj.id+"'>"+obj.name+"</a>";
            var nextpar = document.createElement('ul');
            nextpar.classList.add("sub-menu");
            element.appendChild(nextpar);
            navigation(obj.children ,nextpar);
        }
        else
        {
            var strATag = "<a href='category.html?category="+obj.id+"''>"+obj.name+"</a>";;
            if (obj.id === 10086) 
            {
                strATag = "<a href='/'>"+obj.name+"</a>";
            }
            if (obj.id === 10010)
            {
                strATag = "<a href='/about.html'>"+obj.name+"</a>";
            }
            element.innerHTML = strATag;
        }
        $(parentElement).append(element);
    }
}


axios.all([slider_images(), navigation_bar(), website_options()])
.then(axios.spread(function (resSlider, resNavigation, resWebsiteOptions) 
{
    if (resSlider.data != "null") 
    {
        slider_callback(resSlider);
    }
    
    if (resNavigation.data != "null") 
    {
        navigation_callback(resNavigation);
    }

    if (resWebsiteOptions.data != "null") 
    {
        website_callback(resWebsiteOptions);
    }

    $.getScript("assets/js/plugins.js",function()
    {
        $.getScript("assets/js/scripts.js");
        $("#preloader").fadeOut(1500);
    });
}))
.catch(err=>
{
    $("#preloader").fadeOut(1500);
    spopAlert(err.error, "error", "bottom-right");
    return;
});

