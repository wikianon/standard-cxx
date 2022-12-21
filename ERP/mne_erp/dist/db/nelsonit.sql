SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = mne_crm, pg_catalog;


INSERT INTO country (countrycarcode, countryid, createdate, createuser, modifydate, modifyuser, name_de, phoneprefix, continent_de, name_en, continent_en) VALUES ('CH', '519e03000000', 1369309952, 'admindb', 1369309952, 'admindb', 'Schweiz', '+41', 'Europa', 'switzerland', 'europe');
INSERT INTO city (cityid, countryid, createdate, createuser, modifydate, modifyuser, name, postcode) VALUES ('519e03010000', '519e03000000', 1369309953, 'admindb', 1369309953, 'admindb', 'Gretzenbach', '5014');
INSERT INTO address (addressid, cityid, createdate, createuser, modifydate, modifyuser, postbox, street, refid, addresstypid) VALUES ('519e06240000', '519e03010000', 1369310756, 'admindb', 1369310756, 'admindb', '', '', '519e03290000', '000000000001');
INSERT INTO company (companyid, createdate, createuser, modifydate, modifyuser, name, ownerid, sorting) VALUES ('519e03290000', 1369309993, 'admindb', 1369309993, 'admindb', 'Nelson - technische Informatik', NULL, NULL);
INSERT INTO companydata (createdate, createuser, modifydate, modifyuser, companydataid, telefon, http, email, categorie, language, fax, mobile) VALUES (1369309993, 'admindb', 1369309993, 'admindb', '519e03290000', '062 849 59 91', 'www.nelson-it.ch', 'kontakt@nelson-it.ch', 'vendor', 'de', '', '');


