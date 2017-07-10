module.exports = function (context, eventHubMessages) {
    context.log(eventHubMessages);

    var message = {
        /*"personalizations": [
            {
            "to": [
                {
                "email": "temporary mail address"
                }
            ],
            "subject": "temporary subject"
            }
        ],
        "from": {
            "email": "temporary maill address"
        },*/
        "content": [
            {
            "type": "text/plain",
            "value": eventHubMessages
            }
        ]
    };

    context.done(null, message);
};