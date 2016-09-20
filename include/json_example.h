#include <json-c/json.h>
#include <stdio.h>

/*printing the value corresponding to boolean, double, integer and strings*/
void print_json_value(json_object *jobj){
  enum json_type type;
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean:
      printf(" [json_type_boolean]\t->\t%s\n", json_object_get_boolean(jobj)? "true": "false");
      break;
    case json_type_double:
      printf(" [json_type_double]\t->\t%lf\n", json_object_get_double(jobj));
      break;
    case json_type_int:
      printf(" [json_type_int]\t->\t%d\n", json_object_get_int(jobj));
      break;
    case json_type_string:
      printf(" [json_type_string]\t->\t%s\n", json_object_get_string(jobj));
      break;
  }

}

void json_parse_array( json_object *jobj, char *key) {
  void json_parse(json_object * jobj); /*Forward Declaration*/
  enum json_type type;

  json_object *jarray = jobj; /*Simply get the array*/
  if(key) {
    jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  }

  int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
  printf(" [json_type_array(%d)]\n",arraylen);
  int i;
  json_object * jvalue;

  for (i=0; i< arraylen; i++){
    jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    type = json_object_get_type(jvalue);
    if (type == json_type_array) {
      json_parse_array(jvalue, NULL);
    }
    else if (type != json_type_object) {
      printf("value[%d]: ",i);
      print_json_value(jvalue);
    }
    else {
      json_parse(jvalue);
    }
  }
}

/*Parsing the json object*/
void json_parse(json_object * jobj) {
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    printf("%s", key);
    type = json_object_get_type(val);
    switch (type) {
      case json_type_boolean:
      case json_type_double:
      case json_type_int:
      case json_type_string:
        print_json_value(val);
        break;
      case json_type_object:
        printf(" [json_type_object]\n");
        jobj = json_object_object_get(jobj, key);
        json_parse(jobj);
        break;
      case json_type_array:
        json_parse_array(jobj, key);
        break;
    }
  }
}
#define QUOTE(...) #__VA_ARGS__
int test_json() {
  char * string = QUOTE({
                       "site": "0",
                       "domain": "talk.thetreehole.net",
                       "settings": "{\"key\":\"qLNYkBu8hSDp2zTKgrYIBc+TLVj8933akgVywQFgUxw=\\n\",\"acl\":\"0.0.0.0/0\",\"audibles\":{\"music_on_hold\":\"http://s3.amazonaws.com/teliax.ivy.production/audibles/site_2/site_2/music_on_hold.ogg\"}}",
                       "features": [
                           {
                               "core_id": "27",
                               "core_type": "phone_number",
                               "trigger_value": "17207081501",
                               "values": "{\"number\":\"17207081501\",\"destination\":\"device:57\"}"
                           },
                           {
                               "core_id": "28",
                               "core_type": "phone_number",
                               "trigger_value": "17207081502",
                               "values": "{\"number\":\"17207081502\",\"destination\":\"fax_machine:1\"}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "voice_mail_box",
                               "values": "{\"pin\":\"1776\",\"audibles\":{\"announcement\":\"http://s3.amazonaws.com/teliax.ivy.production/audibles/default/voice_mail_box/announcement.ogg\"},\"notification_email\":\"juston.davies@gmail.com\"}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "ring_group",
                               "values": "{\"destinations\":[\"device:35\",\"device:3\",\"device:1\",\"device:57\"],\"timeout\":\"10\",\"fail_over\":\"forward:1\"}"
                           },
                           {
                               "core_id": "9",
                               "core_type": "pbx",
                               "trigger_value": "test_pbx",
                               "values": "{\"login\":\"test_pbx\",\"password\":\"1234abcd!\",\"caller_id_name\":\"\",\"caller_id_number\":\"\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"10\"}"
                           },
                           {
                               "core_id": "10",
                               "core_type": "pbx",
                               "trigger_value": "test_pbx_1",
                               "values": "{\"login\":\"test_pbx_1\",\"password\":\"1234abcd!\",\"caller_id_name\":\"\",\"caller_id_number\":\"\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"10\"}"
                           },
                           {
                               "core_id": "11",
                               "core_type": "pbx",
                               "trigger_value": "test_pbx_2",
                               "values": "{\"login\":\"test_pbx_2\",\"password\":\"1234abcd!\",\"caller_id_name\":\"\",\"caller_id_number\":\"\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"10\"}"
                           },
                           {
                               "core_id": "12",
                               "core_type": "pbx",
                               "trigger_value": "test_pbx_3",
                               "values": "{\"login\":\"test_pbx_3\",\"password\":\"1234abcd!\",\"caller_id_name\":\"\",\"caller_id_number\":\"\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"10\"}"
                           },
                           {
                               "core_id": "2",
                               "core_type": "device",
                               "trigger_value": "MotorolaXoom",
                               "values": "{\"login\":\"MotorolaXoom\",\"password\":\"BuySomeApples!\",\"caller_id_name\":\"Xoom\",\"caller_id_number\":\"7201111111\",\"timeout\":\"30\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"1\"}"
                           },
                           {
                               "core_id": "35",
                               "core_type": "device",
                               "trigger_value": "MotorolaDroid",
                               "values": "{\"login\":\"MotorolaDroid\",\"password\":\"BuySomeApples!\",\"caller_id_name\":\"\",\"caller_id_number\":\"\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"1\"}"
                           },
                           {
                               "core_id": "3",
                               "core_type": "device",
                               "trigger_value": "1002",
                               "values": "{\"login\":\"1002\",\"password\":\"BuySomeApples!\",\"caller_id_name\":\"Lambda\",\"caller_id_number\":\"3038095226\",\"timeout\":\"30\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"1\"}"
                           },
                           {
                               "core_id": "60",
                               "core_type": "device",
                               "trigger_value": "test",
                               "values": "{\"login\":\"test\",\"password\":\"t3l1ax\",\"caller_id_name\":\"\",\"caller_id_number\":\"\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"1\"}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "device",
                               "trigger_value": "Nexus",
                               "values": "{\"login\":\"Nexus\",\"password\":\"BuyS0meApples!\",\"caller_id_name\":\"Juston Davies\",\"caller_id_number\":\"9706315296\",\"timeout\":\"30\",\"fail_over\":\"device:2\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"25\"}"
                           },
                           {
                               "core_id": "57",
                               "core_type": "device",
                               "trigger_value": "juston_desk",
                               "values": "{\"login\":\"juston_desk\",\"password\":\"BuyS0meApples!\",\"caller_id_name\":\"\",\"caller_id_number\":\"9706315296\",\"timeout\":\"120\",\"acl\":\"0.0.0.0/0\",\"channel_limit\":\"25\"}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "menu",
                               "values": "{\"pin\":\"1776\",\"audibles\":{\"music_on_hold\":\"http://s3.amazonaws.com/teliax.ivy.production/audibles/default/menu/music_on_hold.ogg\",\"greeting\":\"http://s3.amazonaws.com/teliax.ivy.production/audibles/default/menu/greeting.ogg\"},\"options\":{}}"
                           },
                           {
                               "core_id": "5",
                               "core_type": "extension",
                               "trigger_value": "1004",
                               "values": "{\"number\":\"1004\"}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "extension",
                               "trigger_value": "1000",
                               "values": "{\"number\":\"1000\",\"destination\":\"conference_room:2\"}"
                           },
                           {
                               "core_id": "2",
                               "core_type": "extension",
                               "trigger_value": "1001",
                               "values": "{\"number\":\"1001\",\"destination\":\"device:1\"}"
                           },
                           {
                               "core_id": "4",
                               "core_type": "extension",
                               "trigger_value": "1003",
                               "values": "{\"number\":\"1003\",\"destination\":\"device:57\"}"
                           },
                           {
                               "core_id": "3",
                               "core_type": "extension",
                               "trigger_value": "1002",
                               "values": "{\"number\":\"1002\",\"destination\":\"device:2\"}"
                           },
                           {
                               "core_id": "2",
                               "core_type": "conference_room",
                               "values": "{\"pin\":\"1234\",\"profile\":\"default\",\"moderator_pin\":\"1591\",\"audibles\":{\"music_on_hold\":\"http://s3.amazonaws.com/teliax.ivy.production/audibles/site_2/conference_room_2/music_on_hold.ogg\",\"greeting\":\"http://s3.amazonaws.com/teliax.ivy.production/audibles/site_2/conference_room_2/greeting.ogg\"}}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "schedule",
                               "values": "{\"time_zone\":\"Pacific/Midway\",\"start_time\":\"0800\",\"start_weekday\":\"1\",\"end_time\":\"1800\",\"end_weekday\":\"5\",\"exceptions\":\"11/2211/23\",\"options\":{}}"
                           },
                           {
                               "core_id": "13",
                               "core_type": "schedule",
                               "values": "{\"time_zone\":\"America/Denver\",\"start_time\":\"0800\",\"start_weekday\":\"0\",\"end_time\":\"2000\",\"end_weekday\":\"6\",\"exceptions\":\"\",\"options\":{\"positive\":\"forward:24\"}}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "forward",
                               "values": "{\"timeout\":\"30\",\"dial_string\":\"7202313844\"}"
                           },
                           {
                               "core_id": "24",
                               "core_type": "forward",
                               "values": "{\"timeout\":\"120\",\"dial_string\":\"3039735940\"}"
                           },
                           {
                               "core_id": "1",
                               "core_type": "fax_machine",
                               "values": "{\"notification_email\":\"Juston.Davies@gmail.com\"}"
                           }
                       ]
                   });
  printf("JSON string: %s\n", string);
  json_object * jobj = json_tokener_parse(string);
  json_parse(jobj);
}