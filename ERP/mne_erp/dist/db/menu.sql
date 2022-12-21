SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

INSERT INTO mne_catalog.uuid SELECT uuid_in(md5(random()::text || now()::text)::cstring), '1';

INSERT INTO mne_application.folder (createdate, createuser, modifydate, modifyuser, folderid, location, name, description) VALUES (1422026922, 'admindb', 1433858438, 'admindb', '54c268c10000', '/repository', 'repository', 'Repository');
