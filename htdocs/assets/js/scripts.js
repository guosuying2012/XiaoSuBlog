!function(a){jQuery(document).ready(function(){function b(a){var b=/^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$/;return b.test(a)?!0:!1}var c,d,e,f,g,h,i,j,k;jQuery(window).load(function(){jQuery("#preloader").fadeOut(300)}),c=jQuery("#featured"),c.owlCarousel({items:4,pagination:!1}),jQuery("#next-owl").click(function(){c.trigger("owl.next")}),jQuery("#prev-owl").click(function(){c.trigger("owl.prev")}),jQuery(".house-slideshow").length&&jQuery(".house-slideshow").each(function(){jQuery("ul",this).responsiveSlides({auto:!0,pager:!1,nav:!0,prevText:"<i class='fa fa-chevron-left'></i>",nextText:"<i class='fa fa-chevron-right'></i>"})}),jQuery(".fit").fitVids(),d=jQuery("#masonry-1"),d.isotope({itemSelector:".post"}),e=jQuery("#masonry-2"),e.isotope({itemSelector:".post"}),jQuery(window).load(function(){d.isotope("layout"),e.isotope("layout")}),jQuery(".sticky").sticky({topSpacing:0}),jQuery("#menu-container nav > ul > li").each(function(){jQuery(".ubermenu-submenu",this).length>0&&jQuery(this).css("position","static")}),jQuery(".ubermenu-tabs-group > li").hover(function(){var a=jQuery(this).parent();jQuery("> li",a).each(function(){jQuery(this).removeClass("ubermenu-active")}),jQuery(this).addClass("ubermenu-active")}),jQuery("#menu-container li").hover(function(){jQuery(window).width()>991&&jQuery("> .sub-menu, > .ubermenu-submenu",this).stop().fadeIn(300)},function(){jQuery(window).width()>991&&jQuery("> .sub-menu, > .ubermenu-submenu",this).stop().fadeOut(300)}),jQuery("#mobile-button").click(function(){"none"==jQuery("#menu-container").css("display")?jQuery("#menu-container").css("display","block"):jQuery("#menu-container").css("display","none")}),jQuery("#menu-container li").each(function(){jQuery(".sub-menu, .ubermenu-submenu, .ubermenu-tab-content-panel",this).length>0&&jQuery("> a",this).addClass("arrow")}),jQuery("#menu-container a").click(function(){if(jQuery(window).width()<991){var a=jQuery(this).parent();if(jQuery(".sub-menu, .ubermenu-tab-content-panel",a).length>0)return a.hasClass("menu-open")?jQuery(a).removeClass("menu-open"):jQuery(a).addClass("menu-open"),!1;if(jQuery(".ubermenu-submenu",a).length>0)return a.hasClass("menu-open")?jQuery(a).removeClass("menu-open"):jQuery(a).addClass("menu-open"),!1}}),f=jQuery('input[name="personal"]'),g=jQuery('input[name="email"]'),h=jQuery('textarea[name="message"]'),jQuery('#contactform button[type="submit"], #contactform input').removeAttr("disabled"),jQuery("form#contactform").length>0&&jQuery("form#contactform")[0].reset(),jQuery("form#contactform").submit(function(){i=0;var c=jQuery(this).serialize();return""==f.val()?(f.addClass("error"),i++):f.removeClass("error"),""!=g.val()&&b(g.val())?g.removeClass("error"):(g.addClass("error"),i++),""==h.val()?(h.addClass("error"),i++):h.removeClass("error"),0==i&&(jQuery('#contactform button[type="submit"], #contactform input').attr("disabled","disabled"),jQuery("#contactform .loading").slideDown(300),a.ajax({type:"POST",url:"assets/php/sendEmail.php",data:c,success:function(a){"success"==a?(jQuery("#contactform .loading").slideUp(300),jQuery("#success-message").slideDown(500)):(jQuery("#contactform .loading").slideUp(300),jQuery("#error-message").slideDown(500))}})),!1}),jQuery("#style-switcher span").click(function(){var a=jQuery(this).parent().css("left");parseInt(a,10)<0?jQuery(this).parent().addClass("open"):jQuery(this).parent().removeClass("open")}),j=c.data("owlCarousel"),"boxed"==a.cookie("layout")?(jQuery("html").addClass("boxed"),j.reinit(),jQuery('#style-switcher select option:contains("Boxed")').prop("selected",!0)):(jQuery('#style-switcher select option:contains("Wide")').prop("selected",!0),j.reinit(),jQuery("html").removeClass("boxed")),""!=a.cookie("bg")&&null!=a.cookie("bg")&&"wide"!=a.cookie("layout")&&(k=a.cookie("bg"),jQuery("body").addClass(k),jQuery("html").addClass("boxed"),jQuery('#style-switcher select option:contains("Boxed")').prop("selected",!0),j.reinit()),jQuery("#style-switcher img").click(function(){var b=jQuery(this).attr("data-background");jQuery("body").removeAttr("class"),jQuery("body").addClass(b),a.cookie("bg",b,{expires:1}),jQuery("html").addClass("boxed"),a.cookie("layout","boxed",{expires:1}),jQuery('#style-switcher select option:contains("Boxed")').prop("selected",!0),j.reinit()}),jQuery("#style-switcher select").on("change",function(){if("Boxed"==this.value){jQuery("html").addClass("boxed"),j.reinit(),a.cookie("layout","boxed",{expires:1});var b=a.cookie("bg");jQuery("body").addClass(b)}else jQuery("html").removeClass("boxed"),j.reinit(),a.cookie("layout","wide",{expires:1})})})}(jQuery);