module.exports = function (RED) {
    function AlarmGroup(config) {
        RED.nodes.createNode(this, config);
        var node = this;
        node.log("*** ALARM GROUP LOADED");
        //
        // Locally store the scale and offset
        node.name = config.name;
        node.waitTime = parseInt(config.waitTime);
        node.table = new Object(); // table of alarm states
        //
        //
        node.okonvalue = RED.util.evaluateNodeProperty(config.okonvalue, config.okonvalueType, node); // translate to correct type
        node.okoffvalue = RED.util.evaluateNodeProperty(config.okoffvalue, config.okoffvalueType, node); // translate to correct type
        node.alertonvalue = RED.util.evaluateNodeProperty(config.okalertvalue, config.alertonvalueType, node); // translate to correct type
        node.alertoffvalue = RED.util.evaluateNodeProperty(config.alertoffvalue, config.alertoffvalueType, node); // translate to correct type
        node.actiononvalue = RED.util.evaluateNodeProperty(config.actiononvalue, config.actiononvalueType, node); // translate to correct type
        node.actionoffvalue = RED.util.evaluateNodeProperty(config.actionoffvalue, config.actionoffvalueType, node); // translate to correct type
        //
        node.changed = false;
        //    
        node.on('input', function (msg) {

            //
            // topic is the "sample point"
            // the state property is the state
            // store state in table
            //
            node.log("Enter Alarm Group");
            //
            if (msg.topic == '__CLEAR__') {
                this.table = {}; // reset the table
                this.changed = true;
            }
            else {
                // update table
                if (typeof (msg.state) == 'string') {
                    this.table[msg.topic] = msg.state; // save the current state as a property  
                    this.changed = true;
                }
            }
            return null; // do not propogate and messages here
        });

        //
        // timer handler - build the table and send it
        // This avoids too many updates when lots of things change
        //
        node.on("TIX", function (inMsg) {
            if (this.changed) {
                //
                var action = false;
                var alert = false;
                var tableItems = [];


                // iterate the table
                for (var i in this.table) {
                    var v = this.table[i];
                    if (v == 'alert') {
                        alert = true;
                        tableItems.push({ item: i, state: 'alert', status: "yellow" });
                    }
                    else if (v == 'action') {
                        action = true;
                        tableItems.push({ item: i, state: 'action', status: "red" });
                    }
                    else {
                        tableItems.push({ item: i, state: 'ok', status: "green" });
                    }
                }
                //    
                var ok = ((alert || action) == false);
                // 
                // we have four outputs  value, ok, action , alert
                var state = 'ok';
                var statusColour = 'green';
                if (action) {
                    state = 'action';
                    statusColour = 'red';
                }
                else if (alert) {
                    state = 'alert';
                    statusColour = 'yellow';
                }
                //
                // set flags - the messages may drive outputs - so we need on and off for each possible status
                //
                var okMsg = { payload: ok ? this.okonvalue : this.okoffvalue, topic: node.name, value: v, state: state };
                var alertMsg = { payload: alert ? this.alertonvalue : this.alertoffvalue, topic: node.name, value: v, state: state };
                var actionMsg = { payload: action ? this.actiononvalue : this.actionoffvalue, topic: node.name, value: v, state: state };
                //
                if (action) {
                    this.status({ fill: "red", shape: "dot", text: "Action" });
                }
                else if (alert) {
                    this.status({ fill: "yellow", shape: "dot", text: "Alert" });
                }
                else {
                    this.status({ fill: "green", shape: "dot", text: "OK" });
                }
                //
                //
                var tableMsg = { topic: node.name, payload: tableItems, state: state, status: statusColour };
                //
                node.send([tableMsg, okMsg, alertMsg, actionMsg]);
            }
            this.changed = false;
        });


        var tick = setInterval(function () {
            var msg = { payload: 'TIX', topic: "" };
            node.emit("TIX", msg);
        }, 1000 * parseInt(node.waitTime));

        node.on("close", function () {
            if (tick) {
                clearInterval(tick);
            }
        });
    }
    RED.nodes.registerType("alarmgroup", AlarmGroup);
}
