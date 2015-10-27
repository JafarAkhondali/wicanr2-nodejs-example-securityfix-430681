$(document).ready(function(){
    $("button").click(function(){
        if ( this.id == 'btn1' ) {
            $.ajax({url:"hello.txt", success: function(result){
                $("#div1").append(result);
            }});
        }
    });
});
