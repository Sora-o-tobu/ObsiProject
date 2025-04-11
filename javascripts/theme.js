// Set the color attribute immediately to avoid delay
let storedColor = localStorage.getItem("data-md-color-primary") || "custom";
document.body.setAttribute("data-md-color-primary", storedColor);

document.addEventListener("DOMContentLoaded", function() {
    let buttons = document.querySelectorAll("button[data-md-color-primary]");
    buttons.forEach(button => {
        button.addEventListener("click", function() {
            let attr = this.getAttribute("data-md-color-primary");
            if (storedColor !== attr) {
                document.body.setAttribute("data-md-color-primary", attr);
                localStorage.setItem("data-md-color-primary", attr);
                storedColor = attr;
            }
        });
    });
});


// docs/javascripts/transitions.js
document.addEventListener('DOMContentLoaded', function() {
    // 页面切换动画
    setupPageTransitions();
});

function setupPageTransitions() {
    // 监听页面切换事件
    document.addEventListener('DOMContentLoaded', function() {
        document.body.classList.add('page-transition-ready');
    });

    // 页面离开动画
    window.addEventListener('beforeunload', function() {
        document.body.classList.add('page-transition-exit');
    });
}
