<?php

$GLOBALS['_reason_page_types_local'] = array(
		'default' => array(
			'pre_bluebar' => 'textonly_toggle_top',
			'main' => 'content',
			'main_head' => 'page_title',
			'edit_link' => 'login_link',
			'pre_banner' => 'announcements',
			//'banner_xtra' => 'google_search_appliance',
			'banner_xtra' => 'nav_search_logo',
			'post_banner' => 'navigation_top',
			'pre_sidebar' => 'assets',
			'sidebar' => 'luther_image_sidebar',
			'navigation' => 'navigation',
			'footer' => 'maintained',
			'sub_nav' => 'luther_username',
			'sub_nav_2' => 'blurb',
			'sub_nav_3' => '',
			'post_foot' => 'luther_footer',
			'imagetop' => 'luther_imagetop',
		),
		'test_page' => array(
			'main_post' => 'test_module',
		),
		'admissions_account_signup' => array(
			'main_post' => 'applicant_account',
		),
		'luther_news_page' => array(
			'main_post' => 'publication',
            		'sub_nav_3' => 'quote',
		),
		'luther_static_page' => array(
			'pre_banner' => '',
			'post_banner' => '',
		),
		'luther_pageLC' => array(
			'pre_banner' => '',
			'post_banner' => '',
		),
		'luther_pageLRC' => array(
			'pre_banner' => '',
			'post_banner' => '',
		),
		'luther_primaryLRC' => array(
            		'main_post'=>array(  
            			'module' => 'luther_other_publication_news',
				'max_num_to_show' => 3,
				),
			'pre_banner' => '',
			'post_banner' => '',
            		'sub_nav_3' => array( // Spotlights
            			'module' => 'publication',
				'related_publication_unique_names' => array( 'spotlight_archives' ),
				'related_mode' => 'true',
				'related_title' => '',
				'related_order' => 'random',
				'max_num_items' => 1,
				'markup_generator_info' =>
				   array(
				     'list_item' =>
				 	array (
					  'classname' => 'SpotlightListItemMarkupGenerator',
				          'filename' => 'minisite_templates/modules/publication/list_item_markup_generators/spotlight.php'

					      ),
					 ),
				   ),
		),
		'luther_home_page' => array(
            		'main_post'=>'',
            		'main_head' => '',
            		'main'=>'',
            		'sidebar'=>array(  
            			'module' => 'luther_other_publication_news',
				'max_num_to_show' => 5,
				),
            		'pre_sidebar' => array( // Spotlights
            		'module' => 'publication',
				'related_publication_unique_names' => array( 'spotlight_archives' ),
				'related_mode' => 'true',
				'related_title' => '',
				'link_to_full_item' => 'true',
				'related_order' => 'random',
				'max_num_items' => 1,
				'markup_generator_info' =>
				   array(
				     'list_item' =>
				 	array (
					  'classname' => 'SpotlightListItemMarkupGenerator',
				          'filename' =>
'minisite_templates/modules/publication/list_item_markup_generators/spotlight.php'
					      ),
					 ),
				   ),
		),

		'publication' => array(
			'main_post'=>'publication',
            		'main_head' => 'publication/luther_title',
			'main'=>'publication/description',
			'pre_banner' => '',
			'post_banner' => '',
			'sidebar'=>'',
			'pre_sidebar' => '',
        ),        

        //----------------Steve's Spotlight Archive------------------
         'spotlight_archive' => array(
            'main_post' => array( // Spotlights
            	'module' => 'publication',
				'related_title' => '',
				//'link_to_full_item' => 'true',
				'markup_generator_info' =>
				   array(
				     'item' =>
				 	array (
					  'classname' => 'SpotlightItemMarkupGenerator',
				          'filename' =>'minisite_templates/modules/publication/item_markup_generators/spotlight.php'
					      ),
				     'list' =>
				 	array (
					  'classname' => 'SpotlightPublicationListMarkupGenerator',
				          'filename' =>'minisite_templates/modules/publication/publication_list_markup_generators/spotlight.php'
					      ),
				),
        	),     
            'main_head' => 'publication/luther_title',
            'main'=>'publication/description',
		'pre_banner' => '',
		'post_banner' => '',
            'sidebar'=>'',
            //'pre_sidebar' => '',
	),
        //----------------End Steve's Spotlight Archive---------------
        
        //-----------------------Steve's Homepage Begin---------------
        'homepage' => array(
            'main_post'=>'',
            'main_head' => '',
            'main'=>'',
	    'banner_xtra' => 'google_search_appliance',
            'sidebar'=>array(  
            	'module' => 'luther_other_publication_news',
				),
            'pre_sidebar' => array( // Spotlights
            	'module' => 'publication',
				'related_publication_unique_names' => array( 'spotlight_archives' ),
				'related_mode' => 'true',
				'related_title' => '',
				'related_order' => 'random',
				'max_num_items' => 1,
				'markup_generator_info' =>
				   array(
				     'list_item' =>
				 	array (
					  'classname' => 'SpotlightListItemMarkupGenerator',
				          'filename' =>
'minisite_templates/modules/publication/list_item_markup_generators/spotlight.php'
					      ),
					 ),
            //'post_sidebar' => 'bannerAds',
            ),
         ),
        //-----------------------Steve's Homepage End-----------------
        'admissions_home' => array(
		'banner_xtra' => '',
		'navigation' => 'admissions_navigation',	
		'sub_nav_2' => 'admissions_sub_nav_2',
        	'main_post' => 'admissions_main_post',	
		'pre_sidebar' => 'admissions_pre_sidebar',
		'sidebar' => 'admissions_sidebar',
		'post_sidebar' => 'admissions_post_sidebar',
	),
        //-----------------------Steve's EventWithForm Page Begin----------------
        'event_with_form' => array(
        	'main' => 'events',
        	'main_post' => 'form',	
        ),
         
        //-----------------------Steve's EventWithForm End Begin----------------- 
              
		'onecard' => array(
            'main_post'=>'onecard_dashboard',
            'sidebar'=>'',
            'pre_sidebar' => '',
         ),
);

function luther_audience_navigation()
{
        echo '<ul id="navmain">'."\n";
        echo '<li class="nm1"><a href="/admissions">Prospective Students</a></li>'."\n";
        echo '<li class="nm2"><a href="http://www.luther.edu/parents">Parents</a></li>'."\n";
        echo '<li class="nm3"><a href="http://www.luther.edu/visitors">Visitors</a></li>'."\n";
        echo '<li class="nm4"><a href="http://www.luther.edu/alumni">Alumni/ Friends</a></li>'."\n";
        echo '<li class="nm5"><a href="http://www.luther.edu/faculty-staff-students">Faculty/ Staff/ Students</a></li></ul>'."\n";
}

function luther_global_navigation()
{
        echo '<ul id="navglobal">'."\n";
        echo '<li class="ng1"><a href="http://www.luther.edu/academics">Academics</a></li>'."\n";
        echo '<li class="ng2"><a href="/admissions">Admissions</a></li>'."\n";
        echo '<li class="ng3"><a href="http://www.luther.edu/student-life">Student Life</a></li>'."\n";
        echo '<li class="ng4"><a href="http://www.luther.edu/news">News & Events</a></li>'."\n";
        echo '<li class="ng5"><a href="http://www.luther.edu/giving">Giving</a></li>'."\n";
        echo '<li class="ng6"><a href="http://www.luther.edu/about">About Luther</a></li>'."\n";
        echo '<li class="ng7"><a href="http://www.luther.edu/contact">Contact</a></li></ul>'."\n";
}

function luther_google_search()
{
                        echo '<form id="search" action="http://find.luther.edu/search" method="get" name="gs">'."\n";
                        echo '<input type="text" value="" maxlength="256" size="32" name="q" />'."\n";
                        echo '<input id="searchButton" class="button" type="submit" value="Search" name="btnG" />'."\n";
                        echo '<input type="hidden" value="0" name="entqr"/>'."\n";
                        echo '<input type="hidden" value="xml_no_dtd" name="output"/>'."\n";
                        echo '<input type="hidden" value="date:D:L:d1" name="sort"/>'."\n";
                        echo '<input type="hidden" value="public_frontend" name="client"/>'."\n";
                        echo '<input type="hidden" value="1" name="ud"/>'."\n";
                        echo '<input type="hidden" value="UTF-8" name="oe"/>'."\n";
                        echo '<input type="hidden" value="UTF-8" name="ie"/>'."\n";
                        echo '<input type="hidden" value="public_frontend" name="proxystylesheet"/>'."\n";
                        echo '<input type="hidden" value="public_collection" name="site"/>'."\n";
                        echo '<input type="hidden" value="%3CHOME/%3E" name="proxycustom"/>'."\n";
                        echo '</form>'."\n";
}

function admissions_main_navigation()
{
	echo '<div class="main-nav">'."\n";
	echo '<div class="wrap clearfix">'."\n";
	echo '<ul class="nav">'."\n";
	echo '<li class="home"><a href="index.html">Home</a></li>'."\n";
	echo '<li class="fastFacts"><a href="#">Fast Facts</a></li>'."\n";
	echo '<li class="academics"><a href="#">Academics</a></li>'."\n";
	echo '<li class="lifeAtLuther"><a href="#">Life at Luther</a>'."\n";
	echo '<ul>'."\n";
	echo '<li><a href="#">Academics</a></li>'."\n";
	echo '<li><a href="#">Music</a></li>'."\n";
	echo '<li><a href="#">Athletics</a></li>'."\n";
	echo '<li><a href="#">Housing and Dining</a></li>'."\n";
	echo '<li><a href="#">Intramural and Club Sports</a></li>'."\n";
	echo '<li><a href="#">College Ministries</a></li>'."\n";
	echo '<li><a href="#">Diversity Center</a></li>'."\n";
	echo '<li><a href="#">Health Service</a></li>'."\n";
	echo '<li><a href="#">Wellness Program</a></li>'."\n";
	echo '<li><a href="#">Student Activities and Organizations</a></li>'."\n";
	echo '</ul></li>'."\n";
	echo '<li class="lifeAfterLuther"><a href="#">Life after Luther</a></li>'."\n";
	echo '<li class="financialAid active"><a href="section-landing-page.html">Financial Aid</a></li>'."\n";
	echo '<li class="howToApply"><a href="#">How to Apply</a></li>'."\n";
	echo '</ul>'."\n";

	echo '<div class="search">'."\n";
	echo '<input type="text" name="search" id="search" />'."\n";
	echo '<button type="submit"name="submit" id="submit"><img src="/stylesheets/admissions/images/search.png" alt="go!" /></button>'."\n";
	echo '</div>'."\n";
	echo '</div>'."\n";
	echo '</div>'."\n";

	echo '<div class="body wrap clearfix">'."\n";
	
}

?>
