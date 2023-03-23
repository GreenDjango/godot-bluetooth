// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require('prism-react-renderer/themes/github');
const darkCodeTheme = require('prism-react-renderer/themes/dracula');

const isProd = process.env.NODE_ENV === 'production';
const repositoryUrl = 'https://github.com/GreenDjango/godot-bluetooth'
const projectName = 'Godot Bluetooth Module'
const projectShortName = 'GBM'

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: projectName,
  tagline: 'Dinosaurs are cool',
  url: 'https://greendjango.github.io/',
  baseUrl: '/godot-bluetooth/',
  favicon: 'img/favicon.ico',
  trailingSlash: true,

  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'throw',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'GreenDjango', // Usually your GitHub org/user name.
  projectName: 'godot-bluetooth', // Usually your repo name.
  // deploymentBranch: 'gh-pages', // Commented for use CI instead

  // Even if you don't use internalization, you can use this field to set useful
  // metadata like html lang. For example, if your site is Chinese, you may want
  // to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  plugins: [
    '@docusaurus/plugin-content-pages',
    [
      '@docusaurus/plugin-content-docs',
      /** @type {import('@docusaurus/plugin-content-docs').Options} */
      ({
        sidebarPath: require.resolve('./sidebars.js'),
        editUrl: `${repositoryUrl}/tree/main/docs/docs/`,
        showLastUpdateAuthor: false,
        showLastUpdateTime: true,
      })
    ],
    [
      '@docusaurus/plugin-client-redirects',
      /** @type {import('@docusaurus/plugin-client-redirects').Options} */
      {
        redirects: [
          {
            from: '/docs',
            to: '/docs/intro',
          },
        ],
      },
    ],
    !isProd ? '@docusaurus/plugin-debug' : undefined,
  ],

  themes: [
    [
      '@docusaurus/theme-classic',
      /** @type {import('@docusaurus/theme-classic').Options} */
      ({
        customCss: require.resolve('./src/global.css'),
      })
    ],
  ],

  themeConfig:
    /** @type {import('@docusaurus/types').ThemeConfig & import('@docusaurus/theme-common').UserThemeConfig} */
    ({
      image: 'img/card-og.png',
      colorMode: {
        defaultMode: 'dark',
        disableSwitch: false,
        respectPrefersColorScheme: true,
      },
      prism: {
        theme: lightCodeTheme,
        darkTheme: darkCodeTheme,
      },
      docs: {
        versionPersistence: 'localStorage',
        sidebar: {
          hideable: true,
        },
      },
      // announcementBar: {},
      navbar: {
        title: projectName,
        logo: {
          alt: `${projectShortName} Logo`,
          src: 'img/logo.svg',
          srcDark: 'img/logo-dark.svg',
        },
        items: [
          // Left
          {
            type: 'doc',
            docId: 'intro',
            position: 'left',
            label: 'Docs',
          },
          // {to: '/blog', label: 'Blog', position: 'left'},

          // Right
          {
            type: 'docsVersionDropdown',
            position: 'right',
            dropdownActiveClassDisabled: true,
            dropdownItemsAfter: [
              {
                type: 'html',
                value: '<hr class="dropdown-separator">',
              },
              {
                to: '/versions',
                label: 'All versions',
              },
            ],
          },
          {
            type: 'localeDropdown',
            position: 'right',
            dropdownItemsAfter: [
              {
                type: 'html',
                value: '<hr style="margin: 0.3rem 0;">',
              },
              {
                href: `${repositoryUrl}/`,
                label: 'Help Us Translate',
              },
            ],
          },
          {
            href: repositoryUrl,
            position: 'right',
            className: 'header-github-link',
            'aria-label': 'GitHub repository',
          },
        ],
      },
      footer: {
        style: 'dark',
        links: [
          {
            title: 'Docs',
            items: [
              {
                label: 'Tutorial',
                to: '/docs/intro',
              },
            ],
          },
          {
            title: 'Community',
            items: [
              {
                label: 'Stack Overflow',
                href: 'https://stackoverflow.com/questions/tagged/docusaurus',
              },
              {
                label: 'Discord',
                href: 'https://discordapp.com/invite/docusaurus',
              },
              {
                label: 'Twitter',
                href: 'https://twitter.com/docusaurus',
              },
            ],
          },
          {
            title: 'More',
            items: [
              // {
              //   label: 'Blog',
              //   to: '/blog',
              // },
              {
                label: 'GitHub',
                href: repositoryUrl,
              },
            ],
          },
          {
            title: 'Legal',
            items: [
              {
                label: 'Illustrations by Storyset',
                href: 'https://storyset.com/online',
              },
            ],
          },
        ],
        copyright: `Copyright © ${new Date().getFullYear()} ${projectName}. Built with Docusaurus.`,
      },
    }),
};

module.exports = config;
